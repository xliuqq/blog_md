# Spark 序列化

[TOC]

## 序列化方式

### Java Seralizer

`org.apache.spark.serializer.JavaSerializer`

### Kryo Seralizer

`org.apache.spark.serializer.KryoSerializer`

- KryoSerializer中会默认注册来源于Twitter Chill library中的AllScalaRegistrar的类；

- 对于自定义的类，需要自行注册

  ```scala
  val conf = new SparkConf().setMaster(...).setAppName(...)
  conf.registerKryoClasses(Array(classOf[MyClass1], classOf[MyClass2]))
  val sc = new SparkContext(conf)
  ```

## 应用场景

SparkEnv中会创建serializer，closureSerializer和serializerManager三个实例

### serializer

- 通过`spark.serializer`参数指定，默认是`JavaSerializer`；
- 用于以下场景
  - shuffle RDD between executors；
  - RDD serialized cache；
  - Broadcast对象的传输；
  - **ResultTask/ShuffleMapTask的结果的序列化，返回给Driver；**
### closureSerializer
- 闭包清理的序列化工作，是 JavaSerializer；
- Task的序列化；

### serializerManager

- 其成员变量defaultSerializer是SparkEnv中的serializer实例（即根据配置确定）；
- RDD Shuffle时的序列化判断，ShuffleRDD.getDependencies -> SerializerManager.getSerializer
  - 对于ShuffledRDD如果K, V是基本类型、String、Array基本类型，则使用kryo序列化；
  - 否则，使用defaultSerializer

## Task序列化流程

driver端会将Task序列化，并发送给executor，executor进行反序列化。

Task在序列化的时候主要是要**序列化该stage中的最后一个RDD（后面称为finalRDD）**。

在序列化RDD时，需要序列化RDD的成员变量有`RDD id，dependencies_，storageLevel`等，最主要的是`dependencies_`变量，它存放着该RDD的依赖关系，也就是该RDD如何依赖于其他的哪些RDDs，这个依赖关系俗称为**lineage**。

当序列化后的task发到remote worker上时，executor只需要反序列化出finalRDD，然后通过它的lineage就可以从最初的数据源（或者shuffleMapTask的计算结果）一步步计算得到finalRDD。

由于在序列化finalRDD的`dependencies_`时会序列化finalRDD依赖的上游的RDDs的`dependencies_`，那么这个序列化过程实际上是**lineage的回溯过程**。假设lineage是这样的 `RDD a => RDD b => RDD c => finalRDD`，那么首先序列化`finalRDD`，然后顺着dependencies去序列化`RDD c`，然后顺着dependencies去序列化`RDD b`，最后顺着dependencies去序列化`RDD a`。***可见lineage长度越长，序列化链越长，最终可以造成栈溢出。***

- cache不会斩断世系lineage，因此不会影响序列化的长度；
- checkpoint会斩断世系，因为RDD的`dependencies`函数会判断checkpointRDD是否存在，不存在时才会`getDependencies`并设置`dependencies_ `；并且会在checkpoint后，调用`markCheckpointed`函数将`dependencies_`置为空；

**ShuffledRDD可以断掉序列化时的lineage**，也就是说序列化链碰到ShuffledRDD就停止了，这与stage划分方法一致

- ShuffleDependency是不会保存依赖的RDD，但是OneToOneDependency等窄依赖会保存依赖的RDD；

DagScheduler在`submitMissingTasks`函数中序列化（JavaSerializer）stage中的rdd和dependency成为Broadcast变量taskBinary，每个分区映射为一个Task(ShuffleMapTask或ResultTask）包含taskBinary，然后通过TaskScheduler提交TaskSet（封装Array[Task]）。

TaskSetManager的resourceOffer中将Task再次序列化(JavaSerializer)，并返回TaskDescription(encode定义序列化）