# Exceptions

## org.apache.hadoop.hbase.ipc.CallTimeoutException

阻塞超时，可能是批量插入数据过大（HBase正好及进行flush等），导致RPC响应过慢

## org.apache.hadoop.hbase.RegionTooBusyException: Over memstore limit

一般来说memstore超过**hbase.hregion.memstore.flush.size**（默认128M）,会flush形成HFile。

当写入数据过快时，导致产生大量HFile，当HFile数量超过配置hbase.hstore.blockingStoreFiles(默认10)，hbase会进行compaction(合并)，compaction会阻塞memstore flush操作，阻塞最长时长hbase.hstore.blockingWaitTime(默认值90000，即90s)，当超过该时间后，如果compaction还未完成，memstore flush也会停止阻塞。

 但是正是在flush阻塞这段时间内，memstore的大小超过了上限： 

`base.hregion.memstore.flush.size * hbase.hregion.memstore.block.multiplier`（默认为4）=512M，此时，region将拒绝所有写请求，所以客户端抛出RegionTooBusyException，并在一定时间后**重试**。

解决方案：

1.在hbase内存允许的前提下，提高hbase.hregion.memstore.block.multiplier参数，在flush阻塞的这段时间，允许更多的数据写到memstore。

    风险：增加了regionserver oom概率，修改该参数，需要进行大数据量写入测试。

2.减少阻塞时间hbase.hstore.blockingwaittime（比如到30s），加快memstore flush到hdfs上的速度，从而减少memstore数据大小触碰到上限的时间，也就能减少拒绝写请求的时间。

    风险：1、增加了compaction的压力，
         2、占用磁盘IO，可能会影响到其他服务。

3.一般出现RegionTooBusyException，表示大量写入数据量比较大，这种场景更好的选择可能是采用bulkload导入方式。

**bulkload**导入数据到hbase的方式可以参考： https://hbase.apache.org/2.1/book.html#arch.bulk.load.arch 

