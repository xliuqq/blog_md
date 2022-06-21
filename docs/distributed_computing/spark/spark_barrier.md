# RDD Barrier

> [SPIP：Support Barrier Execution Mode in Apache Spark](https://issues.apache.org/jira/browse/SPARK-24374)

**用于MPI或者分布式机器学习的场景，要求所有的Task必须同时同时执行；**



在 barrier stage 中执行 MPI 程序示例：

```scala
rdd.barrier().mapPartitions(iter => {
    // Write iter to disk
    ???
    
    val context = BarrierTaskContext.get()
    // Sets a global barrier and waits until all tasks in this stage hit this barrier
    context.barrier()
    
    if (context.partitionId() == 0) {
        val hosts = context.getTaskInfos().map(_.host)
		// Set up MPI machine file using host infos.
        ???
        
        // Launch the MPI job by calling mpirun.
        ???
    }
    
	// Wait until the MPI job finished.
    context.barrier()
    
    // collect output and return
    ???
})
```

