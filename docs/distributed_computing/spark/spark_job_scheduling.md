[toc]

# Spark 作业调度

## 应用间调度

在多个应用之间调度，但不支持跨应用的内存共享

- Standalone：默认**FIFO**，控制 `spark.cores.max`和 `spark.executor.memory` 控制应用使用的资源；

- Yarn：`--num-executors`，`--executor-memory`，`--executor-cores` 即 `spark.executor.instances`, `spark.executor.memory`, `spark.executor.cores` 控制应用使用的资源；

- Mesos：`spark.mesos.coarse`设置true，`spark.cores.max`和`spark.executor.memory`
  - 另外还支持CPU核的动态共享，当前app没有Task运行，则申请的cores可以被其它app使用



### 动态资源分配

Spark提供了一种基于工作负载动态调整应用程序占用的资源的机制。这意味着，如果资源不再被使用，应用程序可以将它们返回给集群，并在稍后有需要时再次请求它们。支持Standalone，Yarn，Mesos，K8s。两种配置方式：

- `spark.dynamicAllocation.enabled`：true，`spark.dynamicAllocation.shuffleTracking.enabled`：true；
  - Experimental：对于job需要使用的shuffle files所在的executor，不会被释放；
- `spark.dynamicAllocation.enabled`：true，` spark.shuffle.service.enabled`：true；
  - 每个worker节点，会启动`external shuffle service`；





## 应用内调度

单个SparkContext 实例，通过多线程，触发多个action。Spark的调度器是线程安全，支持应用同时服务多个请求。



### 调度策略（FIFO/Fair）

通过Fair或FIFO调度；`spark.scheduler.mode`，默认FIFO

- FIFO：只有前面的job没有占据所有资源时，后续的job才可以运行；
- FAIR：通过**round robin模式在jobs间分配Tasks**，适用于多用户的场景；

配置调度策略

```scala
val conf = new SparkConf().setMaster(...).setAppName(...)
conf.set("spark.scheduler.mode", "FAIR")
val sc = new SparkContext(conf)
```

FAIR支持调度池，将**jobs放到不同的池中**运行，例如，同一个用户可能运行很多job，但是想要用户均分资源而不是job均分资源

```scala
// 对不同线程设置不同的池，这个线程提交的任何job都会使用这个pool name
sc.setLocalProperty("spark.scheduler.pool", "pool1")
```

### 调度配置

默认，每个pool共享集群资源。但**默认的pool中job间是Fair**，**自定义时pool中job间是FIFO**；

- 自定义的pool支持配置，通过fairscheduler.xml文件
  - schedulingMode：FIFO（默认) 或 FAIR
  - weight：默认为1，pool的优先级，越大，其中的Task会越先执行
  - minShare：最小的cpu核数，默认为0

**默认的调度配置文件为`conf/fairscheduler.xml`，可以显示设置**

```scala
conf.set("spark.scheduler.allocation.file", "/path/to/file")
```

文件内容类似：没有在XML文件中配置的任何池将简单地获得所有设置的默认值(调度模式FIFO、权重1和minShare 0)。

```xml
<?xml version="1.0"?>
<allocations>
  <pool name="production">
    <schedulingMode>FAIR</schedulingMode>
    <weight>1</weight>
    <minShare>2</minShare>
  </pool>
  <pool name="test">
    <schedulingMode>FIFO</schedulingMode>
    <weight>2</weight>
    <minShare>3</minShare>
  </pool>
</allocations>
```

JDBC 客户端Session设置Fair Scheduler Pool，可以设置：

```
SET spark.sql.thriftserver.scheduler.pool=accounting;
```

### PySpark 并行作业

在默认情况下**，PySpark不支持同步PVM线程和JVM线程**，并且在**多个PVM线程中启动多个作业并不保证启动每个对应JVM线程中的每个作业**。

为了同步PVM线程和JVM线程，您应该将`PYSPARK_PIN_THREAD`环境变量设置为true。这种**固定线程模式允许一个PVM线程有一个对应的JVM线程**。

注：**PYSPARK_PIN_THREAD目前还处于试验阶段**。