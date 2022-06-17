# Flink

> Apache Flink - Stateful Computations over Data Stream

![Filnk_Flow](./pics/flink-flow.png)

- 批流一体化；
- 精密的状态管理；
- 事件时间支持；
- 精确一次的状态一致性保障；



### 事件驱动型应用



### 数据分析应用



### 数据管道应用



## 架构

由`JobManger`和`TaskManager`组成。

<img src="pics/processes.svg" alt="The processes involved in executing a Flink dataflow" style="zoom:67%;" />

### Client

Client不是运行时和程序执行的一部分，而是用于准备数据流并将数据流发送到JobManager。在此之后，客户端可以断开连接(Detached 模式)，或者保持连接以接收进度报告(Attached 模式)。

### JobManager 

JobManager（类似Spark Driver）有许多与协调Flink应用程序的分布式执行相关的职责:它决定什么时候安排下一个任务(或一组任务)，对完成的任务或执行失败作出反应，协调检查点，协调故障恢复等等。由三部分组成：

- **ResourceManager**：ResourceManager负责Flink集群中的资源分配和发放，**管理slot（资源调度的单元）**，对Yarn、K8s、Standalone都提供不同的实现。**在Standalone模式下，ResourceManager只能分发可用的TaskManager的slots，不能自己启动新的TaskManager**。
- **Dispatcher**：提供了一个**REST接口来提交Flink应用程序**以供执行，并为每个提交的**作业启动一个新的JobMaster**。它还运行Flink web来提供关于作业执行的信息。
- **JobMaster**：JobMaster负责管理单个JobGraph的执行。**多个作业可以在一个Flink集群中同时运行，每个作业都有自己的JobMaster**。

JobManager可以进行高可用配置。

### TaskManagers

TaskManagers(也称为workers)执行数据流的任务，并缓冲和交换数据流。

必须始终至少有一个TaskManager。TaskManager中最小的资源调度单位是任务槽位。**TaskManager的任务槽位数表示并发处理的任务数**。

**注意，多个操作符可以在一个任务槽中执行**。



## 并行策略

### 任务和操作链

对于分布式执行，Flink将操作子任务链成任务（ *chains* operator subtasks together into *tasks*）。

每个Task由一个线程执行，将操作符链接到任务中是一种有用的优化：它减少了线程到线程切换和缓冲的开销，并在降低延迟的同时增加了总体吞吐量。

下图中的样例数据流使用五个子任务执行，因此使用五个并行线程。

![Operator chaining into Tasks](pics/tasks_chains.svg)

### 任务槽和资源（？？）

每个worker (TaskManager)是一个JVM进程，可以在不同的线程中执行一个或多个子任务，能接受的任务数量由`task slots`决定。

**每个任务槽代表TaskManager资源的一个固定子集**。例如，一个有三个槽的TaskManager会将其托管内存的1/3分配给每个槽。对资源进行插槽意味着子任务不会与其他作业的子任务竞争托管内存，而是拥有一定数量的预留托管内存。注意这里没有CPU隔离；目前，槽只分离任务的托管内存。

通过调整任务槽位数，用户可以定义子任务之间的隔离方式。每个TaskManager有一个槽意味着每个任务组在单独的JVM中运行(例如，可以在单独的容器中启动)。拥有多个插槽意味着更多的子任务共享同一个JVM。同一个JVM中的任务共享TCP连接(通过多路复用)和心跳消息。它们还可以共享数据集和数据结构，从而减少每个任务的开销。

<img src="pics/tasks_slots.svg" alt="A TaskManager with Task Slots and Tasks" style="zoom:80%;" />

默认情况下，Flink允许子任务共享槽位，即使它们是不同任务的子任务，只要它们来自同一作业。结果是一个槽可能容纳整个作业管道。允许这种插槽共享有两个主要好处：

- Flink集群需要的任务槽数量与作业中使用的最高并行度完全相同。不需要计算一个程序总共包含多少任务(并行度不同)。
- 更好的资源利用。如果没有插槽共享，非密集型source/map()子任务将阻塞与资源密集型window子任务一样多的资源。使用槽共享，将示例中的基本并行度从2增加到6，可以充分利用槽资源，同时确保繁重的子任务公平地分布在taskmanager中。

![TaskManagers with shared Task Slots](pics/slot_sharing.svg)

## 应用执行

Flink应用程序的作业可以提交给**长时间运行的Flink会话集群、专用的Flink作业集群或Flink应用程序集群（参见[部署模式](./deployment.md)）**。这些选项之间的区别主要与集群的生命周期和资源隔离保证有关。

### Flink Session Cluster

- **集群生命周期**：在Flink会话集群中，客户端连接到一个预先存在的、长时间运行的集群，该集群可以接受多个作业提交。即使在所有作业完成后，集群(和JobManager)仍将继续运行，直到手动停止会话。因此，Flink Session Cluster的生存期不与任何Flink Job的生存期绑定。
- **资源隔离**：TaskManager槽位在作业提交时由ResourceManager分配，作业完成后释放。因为所有作业都共享同一个集群，所以在提交作业阶段存在一些对集群资源的竞争——比如网络带宽。这种共享设置的一个限制是，如果一个TaskManager崩溃了，那么所有在这个TaskManager上运行的任务都会失败；类似，如果JobManager上发生一些致命错误，它将影响集群中运行的所有作业。
- **其他考虑**：使用预先存在的集群可以节省大量申请资源和启动taskmanager的时间。当作业的执行时间非常短，而高启动时间会对端到端用户体验产生负面影响时，这一点非常重要——就像**短查询的交互分析**那样，作业可以使用现有资源快速执行计算。

### Flink Job Cluster

- **集群生命周期**：在Flink作业集群中，可用的集群管理器(如YARN)用于为每个提交的作业启动集群，该集群仅对该作业可用。在这里，客户端首先从集群管理器请求资源以启动JobManager，并将作业提交给运行在此进程中的Dispatcher。然后根据作业的资源需求惰性地分配任务管理器。一旦作业完成，Flink作业集群就会被拆除。
- **资源隔离**：JobManager中的致命错误只影响在该Flink作业集群中运行的一个作业。
- **其他考虑**：因为ResourceManager申请并等待外部资源管理组件启动TaskManager进程和分配资源，link作业集群更适合长期运行、高稳定性要求、启动延迟不敏感的作业。

### Flink Application Cluster

- **集群生命周期**：一个Flink应用程序集群是一个专用的Flink集群，它只执行来自一个Flink应用程序的任务，其中main()方法运行在集群而不是客户端上。将应用程序逻辑和依赖打包到一个可执行的作业JAR中，并且集群入口点（ApplicationClusterEntryPoint）负责调用main()方法来提取JobGraph。例如，这允许您像在Kubernetes上部署其他应用程序一样部署Flink应用程序。Flink应用程序集群的生命周期与Flink应用程序的生命周期绑定在一起。

- **资源隔离**：在一个Flink应用程序集群中，ResourceManager和Dispatcher被限定为一个Flink应用程序，这提供了比Flink会话集群更好的关注点分离。

  
