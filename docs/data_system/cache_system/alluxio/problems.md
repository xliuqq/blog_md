# 疑问

## ReadType

Reading data with the `ReadType.CACHE_PROMOTE` will ensure the data is **first transferred to the top tier from lower tier before it is read from the worker**.  



- alluxio架构图中，client跟worker间，有没有`control path`，在**架构**章节和**调优**章节，其架构图不一致？



* Read Cache Miss的情况

  当只读取block部分或者非顺序访问block，client会指示worker异步cache整个block

  **1) 如果读取整个block，cache的情况是怎么样？**
  
  **2) 读取UFS中数据，一定是client所在的local worker么？**



* `alluxio.user.file.metadata.load.type`: This property can be set to either `ALWAYS`,`ONCE`, or `NEVER`. It acts similar to `alluxio.user.file.metadata.sync.interval`, but with two caveats:
  * It only discovers new files and does not reload modified or deleted files.
  * It only applies to the exists, list, and getStatus RPCs.

- 往alluxio写数据，会被GC么？



- **调优**章节，Alluxio 2.0用grpc替换了thirft，所以涉及到的thrift的地方都需要改动？



一个节点只能启动一个Worker么？



alluxio调优中Master为什么没有线程数的调优了？

- alluxio.master.rpc.executor.max.pool.size的设置，默认500



定位策略：

- 随机选择的容量还不够，怎么办
- 容量是怎么判断，分层情况下，是总容量，还是？



client有worker和没worker的情况下，readtype的cache_promote表现是一致的么？

- 核心是，ReadType的配置和worker本地存储的关系是独立的还是？

  

NO_CACHE即使在已经缓存的情况下，也不会从UFS读取么？



alluxio secondary master checkpoint的时候，需要停服？



**HA模式下，主master的journal会无限制增长**，因此必须要停服？

单个master情况下，Journal Size会无限制地增长，通过HA让standby master执行checkpoint master state and clean up the logs that were written prior to the checkpoints.

primary master不会执行checkpoint，因为checkpoint的时候会造成master metadata 不可改变，因此不能提供服务。因此会导致primary master的journal log无限增长。