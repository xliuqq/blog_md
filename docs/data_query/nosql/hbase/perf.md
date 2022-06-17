# Performance

- 判断单region是否存在读写热点？
  - 

## Schema Design

为什么RowKey不要使用递增

- ID自增长，会导致写入数据压力集中在某一个或某几个Region上；

### Tall/Wide/Middle Schema Design

参考指导而不是规定，需要根据实际需要考虑

- 时间戳作为rowkey，不要作为版本（version）；
- 偏向于窄表；opentsdb将timestamp变成timerange（Row as Columns)，列为时间偏移，



### 避免数据热点

Reversing

Salting

Hashing



### 二级索引RowKey

思路：添加RowKey设计，数据会重复存储、性能下降。



## GC

RegionServer内存小于4GB，使用并行+并发回收器

- 并行回收器ParallelGC,主要针对年轻代进行优化(jdk8默认策略)

- 并发回收器ConcMarkSweepGC,CMS,主要针对年老带进行优化

```shell
export HBASE_REGIONSERVER_OPTS="$HBASE_REGIONSERVER_OPTS -Xms8g -Xmx8g -XX:+UseParNewGCXX:+UseConcMarkSweepGC"
```


内存大于32GB，使用G1GC策略

  ```shell
export HBASE_REGIONSERVER_OPTS="$HBASE_REGIONSERVER_OPTS -Xms8g -Xmx8g -XX:+UseParNewGCXX:+UseConcMarkSweepGC"
  ```

测试的时候，加上调试参数

```shell
-XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+PrintAdaptiveSizePolicy
```

### Memstore的专属JVM策略MSLAB

CMS算法产生Full GC原因：

- 同步模式失败：
  - 在CMS还没有把垃圾收集完的时候空间还没有完全释放,在这个时候如果新生代的对象过快的转换为老年代的对象时发现老年代的可用空间不够用了,此时收集器就会停止收集过程,转为单线程的STW暂停,将又触发Full GC；
  - `-XX:CMSInitiatingOccupancyFraction=N`来缓解,N代表了当JVM启动垃圾回收时的堆内存占用百分比,设置的越小,JVM越早启动垃圾回收进程
- 内存碎片化：
  - 当前要从新生代提升到老年代的对象比老年代的所有可以使用的连续的内存空间都大

解决内存碎片化问题：

- JVM有基于线程的解决方案 TLAB（thread-local allocation buffer），固定大小的内存空间给线程使用；
- HBase中多个region由一个线程管理，多个Memstore占用的空间无法合理分开，HBase自实现 MSLAB（MemStore-local Allocatoin Buffer）；
- MSLAB的实现：
  - RegionServer中维护着一个全局的MemStoreChunkPool实例,是一个chunk池
  - 每个MemStore实例里面有一个MemStoreLAB实例
  - <font color="red">TODO：分析具体流程</font>