[toc]

# OpenTSDB配置优化

## TSDB 内存

### UID caches

维护UID到String（供查找），以及String到UID的映射（供写入）；

### tsd.storage.enable_appends

设置为false，append模式下，tsd并发高时，append的value需要时间戳和数据的拼接，会出现频繁且长时间的GC，opentsdb线程卡死。

### tsd.storage.hbase.prefetch_meta

设置为true，预取hbase元数据

### tsd.storage.hbase.scanner.maxNumRows

增大单次scan读取的

### tsd.storage.salt.buckets，tsd.storage.salt.width

增大负载均衡

### tsd.rollups.enable

读时优化



## HBase存储优化

### Date Tierd Compaction

设置基于时间的压缩



### HBase Read/Write Queues

HBase支持将单独的读/写的RPC队列。

- **hbase.ipc.server.callqueue.read.ratio=0.60**
- **hbase.ipc.server.max.callqueue.length=100**
- **hbase.ipc.server.read.threadpool.size=2**，增加线程数时，减少队列长度；



### HBase Cache

使用堆外缓存

-  **hbase.bucketcache.combinedcache.enabled=true**；
- **hbase.bucketcache.ioengine=offheap**；

尝试以下配置

```properties
hbase.rs.cacheblocksonwrite=true
hbase.rs.evictblocksonclose=false
hfile.block.bloom.cacheonwrite=true
hfile.block.index.cacheonwrite=true
hbase.block.data.cachecompressed=true
hbase.bucketcache.blockcache.single.percentage=.99
hbase.bucketcache.blockcache.multi.percentage=0
hbase.bucketcache.blockcache.memory.percentage=.01
hfile.block.cache.size=.054 #ignored but needs a value.
```

### HBase Compaction

调整要压缩的线程数和阈值，以避免在应将重点放在写入上时使用太多资源。

```properties
hbase.hstore.compaction.ratio=1.2
hbase.regionserver.thread.compaction.large=2
hbase.regionserver.thread.compaction.small=6
hbase.regionserver.thread.compaction.throttle=524288000
```

### HBase Regions

**hbase.hregion.max.filesize** 设置为10G，10737418240

### HBase Memstore

对于**高度写负载，尝试更经常将mem store刷新到磁盘中，并减少memstore的大小（存疑？？）**。

- **hbase.hregion.memstore.flush.size=16777216**
- **hbase.regionserver.global.memstore.lowerLimit=.20，hbase.regionserver.global.memstore.upperLimit=.30**.

### HBase GC

默认情况下HBase在新代中采用的GC方式是UseParNewGC，在老代中采用的GC方式为UseConcMarkSweepGC，这两种GC方法都支持多线程的方法，CMS的GC耗费的时间比新代中的GC长，同时如果内存占满还会触发Full GC，优化方向是让GC尽量在新代中进行，通过GC日志发现新代的内存大小只有600M，而总的Java堆栈大小为8G，**官方的推荐是新代内存占用为总堆栈的3/8**，于是在这里增加参数-Xmn3000m，来扩大新代的大小。

