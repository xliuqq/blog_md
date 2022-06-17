# RDD的action的执行流程：

## RDD action

RDD.Action -> SparkContext.runJob -> DagScheduler.runJob -> submitJob -> 提交JobSubmitted事件

DAGSchedulerEventProcessLoop线程 onReceive -> dagScheduler.handleJobSubmitted -> createResultStage，submitStage 

- createResultStage     -> getOrCreateParentStages -> getShuffleDependencies（会触发new     ShuffleDependency）, getOrCreateShuffleMapStage

- - getOrCreateShuffleMapStage ->      getMissingAncestorShuffleDependencies（获取未创建的父的Shuffle依赖）, createShuffleMapStage -> getOrCreateParentStages（递归）
  - 递归创建ShuffleMapStage，如果都是窄依赖，则计算getShuffleDependencies会一直到初始的RDD；

- submitStage -> activeJobForStage, getMissingParentStages -> submitMissingTasks -> getPreferredLocs, taskScheduler.submitTasks

- - submitStage 会判断stage是否在waiting,      running, failed中；
  - submitMissingTasks 会获取RDD的分区的preferredLocs；
  - submitMissingTasks的序列化；

<font color="blue">TODO：submitTasks之后的动作</font>

<font color="red">如何RDD的preferredLocs</font>，计算的位置：

1）如果有Cache，则获取Cache的节点（Executor）位置；

2）如果是Input RDD或者Shuffle RDD，则通过RDD的preferredLocations计算；

3) 如果是窄依赖的RDD，则获取其父RDD的preferredLocs；

 

handleJobSubmitted时cacheLocs保存cached RDD的位置，每次Job开始的时候，清空Map，然后再getCachedLocs函数中进行赋值。



一个Stage属于多个JobIds?? submitStage时候，会判断 activeJobForStage



## RDD cache
**cache操作必须在action操作之前执行**，可以根据其执行过程推断出原因。

### 执行过程
FinalRDD在计算调用compute函数时，RDD会调用父RDD的iterator方法，其中***iterator会先判断cache，再checkpoint，最后再compute***；
- 第一次计算的时候，会根据StorageLevel将数据保存在BlockManager中（getOrElseUpdate函数）；
- unpersist时，会触发BlockManagerMaster的removeRDD方法；

## RDD checkpoint

RDD checkpoint会重新计算一次RDD，见 SPARK-8582，因此推荐在checkpoint前，先进行cache。

在执行RDD action计算时，在`SparkContext.runJob`最后会执行`rdd.doCheckpoint`进行RDD的checkpoint。