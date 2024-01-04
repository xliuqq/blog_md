# 检查点

Flink中的函数和状态可以是有状态的，有状态函数在处理单个元素/事件时存储数据，使状态成为任何类型更精细操作的关键构建块。

为了使状态容错，Flink需要对状态设置检查点。**检查点允许Flink恢复流中的状态和位置**，为应用程序提供与无故障执行相同的语义。



**Flink从checkpoint恢复，需要手动指定检查点的位置（到chk-的目录）**



## 前置要求

Flink的检查点机制，对流和状态需要可靠的存储，如：

- 一种持久或永久的数据源，可以在一定时间内重播记录，如Kafka、RocketMQ、HDFS、Ceph等；
- 一种持久化的存储，用于存储状态信息，如分布式系统（HDFS、Ceph等）；



## 配置

默认，检查点是关闭。`StreamExecutionEnvironment`的`enableCheckpointing(n) `函数，`n`表示检查点的间隔（ms）。

- *checkpoint storage*：默认使用 JobManager的内存，生产环境建议配置持久化存储；
- *exactly-once vs. at-least-once*：持久化时可以选择不同的语义；
- ***checkpoint timeout***：持久化超时时间，即给定的时间内未完成持久化操作；
- ***minimum time between checkpoints***：两次持久化的最小时间（应该比间隔要大），用于防止存储系统变慢的情况；
- *number of concurrent checkpoints*：并发的检查点写入数，默认为1，当pipeline中有一定的处理延迟；
- ***externalized checkpoints***：外部化检查点将它们的元数据写入持久存储，**当作业失败时不会自动清除**。如果工作失败，有了一个重新开始的检查点；
- *fail/continue task on checkpoint errors*：checkpoint失败是否会导致task失败；
- *prefer checkpoint for recovery*：这将决定作业是否会回退到最新的检查点，即使有更近期的保存点可用来潜在地减少恢复时间。
- *unaligned checkpoints*：启用未对齐的检查点，以大大减少反压力下的检查点时间。只适用于`exactly once`的检查点，并且并发检查点的数量为1。



| Key                                | Default | Type       | Description                                                  |
| :--------------------------------- | :------ | :--------- | :----------------------------------------------------------- |
| state.backend.incremental          | false   | Boolean    | 增量的检查点，而不是每次都是全量的检查点                     |
| state.backend.local-recovery       | false   | Boolean    | 缺省情况下，本地恢复处于去激活状态。本地恢复目前只覆盖关键状态后端。目前，MemoryStateBackend和HashMapStateBackend不支持本地恢复，忽略此选项 |
| state.checkpoint-storage           | (none)  | String     | 检查点存储的实现，内置 'jobmanager' and 'filesystem'         |
| state.checkpoints.dir              | (none)  | String     | 默认的目录，需要所有flink参与的进程可以访问                  |
| state.checkpoints.num-retained     | 1       | Integer    | 保留的完成的checkpoint的数量                                 |
| state.savepoints.dir               | (none)  | String     | savepoint的默认目录，用于状态后端写savepoint到文件系统中(HashMapStateBackend, EmbeddedRocksDBStateBackend). |
| state.storage.fs.memory-threshold  | 20 kb   | MemorySize | 状态数据文件的最小大小。所有比它小的状态块都内联存储在根检查点元数据文件中。此配置的最大内存阈值是1MB。 |
| state.storage.fs.write-buffer-size | 4096    | Integer    | 写入文件系统的检查点流的写入缓冲区的默认大小。实际的写缓冲区大小是这个选项和选项'state.storage.fs.memory-threshold'的最大值。 |
| taskmanager.state.local.root-dirs  | (none)  | String     | 定义根目录的配置参数，用于存储本地恢复的基于文件的状态。本地恢复目前只覆盖关键状态后端。目前，MemoryStateBackend不支持本地恢复，忽略此选项 |



## 检查点存储介质选择

Flink的检查点机制在计时器和有状态操作符(包括连接器、窗口和任何用户定义的状态)中存储所有状态的一致快照。存储检查点的位置(例如，JobManager内存、文件系统、数据库)取决于配置的检查点存储。

默认使用 JobManager的内存，生产环境建议配置高可用的持久化存储；

### JobManagerCheckpointStorage 

如果检查点超过一定的大小，可以将其配置为使检查点失败，以避免JobManager上出现OutOfMemoryError错误。

```java
new JobManagerCheckpointStorage(MAX_MEM_STATE_SIZE);
```

- 每个状态的大小默认**限制为5mb**，可以在`JobManagerCheckpointStorage`的构造函数中增加；
- 不考虑配置的最大状态大小，状态不能大于Akka帧大小(默认10MB)；
- 聚合状态必须适合JobManager内存。

仅适用于：

- 本地开发和调试
- 使用很少状态的作业，例如只包含一次记录函数(Map, FlatMap, Filter，…)的作业。Kafka消费者需要很少的状态。

### FileSystemCheckpointStorage 

在检查点时，它将**状态快照写入配置的文件系统和目录中的文件**。最小的元数据存储在JobManager的内存中(或者在高可用性模式下存储在元数据检查点中)。

推荐用于：

- 所有高可用性设置。



## State Backend

用数据流API编写的程序通常以各种形式保持状态:

- Windows收集元素或聚合，直到它们被触发；
- 转换函数可以使用键/值状态接口来存储值；
- 转换函数可以实现CheckpointedFunction接口，使它们的局部变量容错。

**当检查点被激活时，这种状态将在检查点上持久化，以防止数据丢失并一致地恢复**。状态在内部是如何表示的，以及它在检查点上是如何以及在哪里持久化的，都取决于所选择的State Backend。

Flink内置：默认不配置是`HashMapStateBackend`

- *HashMapStateBackend*
- *EmbeddedRocksDBStateBackend*

### HashMapBackend

HashMapStateBackend将数据作为Java堆上的对象在内部保存。键/值状态和窗口操作符保存存储值、触发器等的哈希表。



### EmbeddedRocksDBStateBackend 



### Legacy

Flink 1.13开始，从概念上本地`state storage`和`checkpoint storage`分开，但实现上没有区别。

MemoryStateBackend 等价于 HashMapBackend 和  JobManagerCheckpointStorage；

FsStateBackend 等价于 HashMapBackend 和   FileSystemCheckpointStorage.；

MemoryStateBackend 等价于 EmbeddedRocksDBStateBackend 和  JobManagerCheckpointStorage；



## 迭代Job的状态检查点（TODO）

