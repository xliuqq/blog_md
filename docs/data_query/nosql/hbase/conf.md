# HBase 配置项

## 系统要求

- NTP：集群节点的时钟应该同步

- ulimit：设置-n和-u，用户同时可以打开文件数和进程数

- 每个regionserver打开的文件数
  ```
  (StoreFiles per ColumnFamily) * (regions per RegionServer)
  ```
- 集群部署，需要将HBase的lib下hadoop的jar包替换成实际使用的hadoop版本的jar包

- hdfs的DataNode对随时提供服务的文件数量有上限，通过以下配置（需要重启HDFS）
  ```xml
  <!--> hdfs-site.xml, hdfs默认是4096<!-->
  <property>
    <name>dfs.datanode.max.transfer.threads</name>
    <value>4096</value>
  </property>
  ```

- 如果Hadoop集群修改了HDFS客户端的配置，在HBase中需要同步，以便可以同样使用这些配置：
  - *hbase-env.sh*配置*HADOOP_CONF*目录到*HBASE_CLASSPATH*中
  - 将*hdfs-site.xml*复制或链接到*${HBASE_HOME}/conf*下
  - 直接将配置添加到*hbase-site.xml*中

## HBase Default Conf

修改配置，需要重启HBase集群

```
hbase.tmp.dir
hbase.rootdir
hbase.cluster.distributed
hbase.zookeeper.quorum
hbase.zookeeper.property.dataDir
...
```

堆内存的设置

```shell
# 该配置对Master和RegionServer都生效
export HBASE_HEAPSIZE=4G
# 该配置只对Master生效
export HBASE_MASTER_OPTS="$HBASE_MASTER_OPTS -Xms4g -Xmx4g"
# 该配置只对RegionServer生效
export HBASE_REGIONSERVER_OPTS="$HBASE_REGIONSERVER_OPTS -Xms8g -Xmx8g"

```



hfile.block.cache.size ： 默认0.4，StoreFile中用来分配Block Cache的最大堆的百分比



1. **hbase.regionserver.global.memstore.size**：默认堆大小的40%

regionServer的全局memstore的大小，超过该大小会阻塞更新且强制flush到磁盘

2. **hbase.hregion.memstore.flush.size**：默认128M

单个region里memstore的缓存大小，超过那么整个HRegion就会flush， 每` hbase.server.thread.wakefrequency `（默认10000ms）检查一次

3. **hbase.regionserver.optionalcacheflushinterval**：默认1h

内存中的文件在自动刷新之前能够存活的最长时间 

4. **hbase.regionserver.global.memstore.size.lower.limit**：默认堆大小 * 0.4 * 0.95

有时候集群的“写负载”非常高，写入量一直超过flush的量，这时，我们就希望memstore不要超过一定的安全设置。在这种情况下，写操作就要被阻塞一直到memstore恢复到一个“可管理”的大小, 这个大小就是默认值是堆大小 * 0.4 * 0.95，也就是当regionserver级别的flush操作发送后,会阻塞客户端写,一直阻塞到整个regionserver级别的memstore的大小为 堆大小 * 0.4 *0.95为止 

5. **hbase.hregion.preclose.flush.size**：默认5M

当一个 region 中的 memstore 的大小大于这个值的时候，我们又触发 了 close.会先运行“pre-flush”操作，清理这个需要关闭的memstore，然后 将这个 region 下线。当一个 region 下线了，我们无法再进行任何写操作。 如果一个 memstore 很大的时候，flush 操作会消耗很多时间。"pre-flush" 操作意味着在 region 下线之前，会先把 memstore 清空。这样在最终执行 close 操作的时候，flush 操作会很快。 

6. **hbase.hstore.compactionThreshold**：默认超过3个

一个store里面允许存的hfile的个数，超过这个个数会被写到新的一个hfile里面 也即是每个region的每个列族对应的memstore在fulsh为hfile的时候，默认情况下当超过3个hfile的时候就会 对这些文件进行合并重写为一个新文件，设置个数越大可以减少触发合并的时间，但是每次合并的时间就会越长

## HBase 内存规划

HBase内存分为**堆内**和堆外内存

**HEAPSIZE = LRUBlockCache + MemStore + Others** 

**OFFHEAPSIZE  = BucketCache + Others**

**LRUBlockCache + MemStore < < 80% * JVM_HEAP** 

假设系统内存为96G，HBase使用80G

HEAP：30G = 0.3 + 0.5

OFFHEAP：50G

BucketCache： 45G

```shell
export HBASE_HEAPSIZE=30720
export HBASE_OFFHEAPSIZE=51200
export HBASE_OPTS="-XX：MaxPermSize=256m -XX：+UseConcMarkSweepGC -XX：+UseParNewGC"
```



```xml
<!--节点上所有region的memstore之和达到upperLimit*heapsize -->
  <property>
    <name>hbase.regionserver.global.memstore.size</name>
    <value>0.5</value>
  </property>
  <property>
    <name>hfile.block.cache.size</name>
    <value>0.3</value>
  </property>
  <property>
    <name>hbase.bucketcache.ioengine</name>
    <value>offheap</value>
  </property>
  <property>
    <name>hbase.bucketcache.size</name>
    <value>39936</value>
  </property>

```



### MSLAB

MSLAB相关参数

- `hbase.hregion.memstore.mslab.enabled`：设置为true，即打开 MSLAB，默认为true
- `hbase.hregion.memstore.mslab.chunksize`：每个chunk的大 小，默认为2048*1024 即2MB
- `hbase.hregion.memstore.mslab.max.allocation`：能放入chunk 的最大单元格大小，默认为256KB，已经很大了
- `hbase.hregion.memstore.chunkpool.maxsize`：在整个memstore 可以占用的堆内存中，chunkPool占用的比例，该值为一个百分 比，取值范围为0.0~1.0。默认值为0.0
- `hbase.hregion.memstore.chunkpool.initialsize`：在 RegionServer启动的时候可以预分配一些空的chunk出来放到 chunkPool里面待使用.该值就代表了预分配的chunk占总的 chunkPool的比例，该值为一个百分比，取值范围为0.0~1.0，默 认值为0.0



### Region Split

#### region拆分策略

参数：**hbase.regionserver.region.split.policy**

默认：**org.apache.hadoop.hbase.regionserver.SteppingSplitPolicy**



策略：**ConstantSizeRegionSplitPolicy**

准则：Region的HFile大小超过该值，则会拆分Region。对小表不友好。

```xml
<property>
  <name>hbase.hregion.max.filesize</name>
  <value>10737418240</value>
</property>
```



策略：**IncreasingToUpperBoundRegionSplitPolicy**

准则：限制不断增长的文件尺寸，文件尺寸限制是动态的。弥补**ConstantSizeRegionSplitPolicy**的短板，自适应大表和小表；但在大集群下对小表会产生大量小region，分散在整个集群中。

```
Math.min(tableRegionCount^3 * initialsize, regionMaxFileSize)
```

- tableRegionCount：在RegionServer上表所拥有的Region数量总和；
- initialSize：`hbase.increasing.policy.initial.size`的值，未设置则默认是memstore的刷写大小2倍；
- regionMaxFileSize 的最大大小，即`hbase.hregion.max.filesize`

 

策略：**SteppingSplitPolicy**

准则：如果region次数等于1，切分阈值为flush_size * 2，否则为regionMaxFileSize。小表不会在产生大量的小region。



策略：**KeyPrefixRegionSplitPolicy**

准则：**IncreasingToUpperBoundRegionSplitPolicy**子类，拆分点(即Region被拆分处的rowkey)，保证了有相同前缀的rowkey拆分到一个region中

```
KeyPrefixRegionSplitPolicy.prefix_length rowkey:前缀长度
```

根据定义的长度来截取rowkey作为分组的依据，同一组的数据不会被划分到不同的region上；
这种策略对于只有一两个前缀的数据，那么根本不需要用这个，这个适用场景在于数据有多种前缀，并且查询多是针对前缀，比较少跨越多个前缀来查询数据。



策略：**DelimitedKeyPrefixRegionSplitPolicy**

准则：**KeyPrefixRegionSplitPolicy**子类，根据rowkey的分隔符进行切分

```xml
DelimitedKeyPrefixRegionSplitPolicy.delimiter : 前缀分隔符
```



策略：**BusyRegionSplitPolicy**

准则：根据热点进行拆分

- `hbase.busy.policy.blockedRequests`：请求被阻塞的严重程度，取值范围是0.0~1.0，默认是0.2，即20%的请求被阻塞的意思

- `hbase.busy.policy.minAge`：拆分最小年龄，当Region的年龄比这个小的时候不拆分，这是为了防止在判断是否要拆分的时候出现了短时间的访问频率波峰，结果没必要拆分的Region被拆分了，因为短时间的波峰会很快地降回到正常水平。单位毫秒，默认值10分钟
- `hbase.busy.policy.aggWindow`：计算是否繁忙的时间窗口，单位毫秒，默认值5分钟，用以控制计算的频率

```python
if 当前时间减去上次检测时间>=hbase.busy.policy.aggWindow :
	请求的被阻塞率aggBlockedRate=这段时间的阻塞的请求/这段时间的总请求
    if aggBlocedRate>hbase.busy.policy.blockedRequests:
        return BUSY
    
```



策略：**DisabledRegionSplitPolicy**

准则：表示region永远不拆分，但是可以通过手动拆分region。



#### region预拆分

在建表的时候定义好拆分点，使用RegionSplitter类来建表，并传入拆分点算法。

```shell
hbase(main):005:0> create 'mysplittable','cf',{NUMREGIONS=>10,SPLITALGO=>'HexStringSplit'}
```

 TODO：拆分点算法



### Region Compaction

#### Minor Compaction

#### Major Compaction



策略：**DateTieredCompaction**

