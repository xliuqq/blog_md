# Spark下的Hadoop配置

默认的HDFS地址，不需要配置 ip:port



## Task

### spark.excludeOnFailure

Spark 针对 Task 失败有重试机制，但是当一个 Task 在某一台 host上的 一个 Spark Executor 实例执行失败，下一次重试调度因为考虑 `Data Locality` 还是会大概率的选择那个 host 上的 Executor。如果失败是因为 `机器坏盘`引起的，那重试还是会失败，重试次数达到最大后那最终整个 Job 失败。而开启 blacklist 功能可以解决此类问题，将发生失败的 Executor 实例或者 host 添加到黑名单，那么重试可以选择其他实例或者 host ，从而提高任务的 `容错`能力。

