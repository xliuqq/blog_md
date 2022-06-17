# 架构

## Overview

HBase的特性：

- 强一致性的读/写：HBase不是最终一致性的数据存储(DataStore)， high-speed counter aggregation；
- 自动的分片：表分布由regions构成，regions自动分裂且自动在集群中重新分布；
- 自动RegionServer 容错；
- Hadoop/HDFS继承：使用HDFS作为底层存储的分布式文件系统；
- 

## Regions

### Store

A Store = A MemStore + 0 or more StoreFiles ( HFiles ) 

​              =  A column family for a table for a given region

#### MemStore

保存对于Store的在内存中的修改，修改是以KeyValue/Cell为单位。当flush时，MemStore会形成一个快照并清空，且**region下的所有MemStores都会被flush**。

**HBase continues to serve edits from the new MemStore and backing snapshot until the flusher reports that the flush succeeded.**  

每个region的**memstore的个数 = 表的列族的个数** 

#### MemStore Flush

触发 Flush需要满足以下任一条件：最小的flush单元是region，not at individual MemStore Level

- 当一个MemStore达到**hbase.hregion.memstore.flush.size**
  - 所有属于该region的memstore都会被flush到磁盘；
  - 如果数据超过**hbase.hregion.memstore.flush.size** * **hbase.hregion.memstore.block.multiplier**，则会阻塞所有写入Store的请求；
- 当所有MemStore的用量超过**hbase.regionserver.global.memstore.size**
  - 多个regions的MemStores会被flush到磁盘减少一个RegionServer的整体MemStore用量；
  - flush的顺序是按照region的MemStore用量降序；
  - Regions会flush自己的MemStores，直到整体的MemStore用量小于等于**hbase.regionserver.global.memstore.size.lower.limit**；
- RegionServer的WAL log的数量超过**hbase.regionserver.max.logs**
  - 多个regions的MemStores会被flush到磁盘来减少WAL log数量；
  - flush的顺序是根据时间；最老的MemStores的Regions首先被flush，直到WAL log数小于阈值；

<font color="red">TODO：刷写的逻辑，和阻塞的判断？？</font>

#### Scans



#### StoreFile(HFile)

StoreFile是数据存活的地方



#### HFile合并策略

minor compaction

major compaction

**ExploringCompactionPolicy**：把所有的HFile都遍历一遍之后才会去考虑合并哪个HFile,符合合并条件的计算公式是

```python
if current_hfile_size < hbase.hstore.compaction.min.size :
	return "Compaction"
if current_hfile_size > hbase.hstore.compaction.max.size:
	return "NoCompaction"

# TODO: 详细的规则
if current_hfile_size < (sum_hfile - current_hfile_size) * 比例因子
```

TODO：其他策略



### Region Split

Region Split 的配置见[Region_Split_Configuration](conf.md#Region Split)

#### 切分点的定义

整个region中最大store中（不同列族）的最大文件中（不同storeFile）最中心的一个block的首个rowkey。

注意：如果定位到的rowkey是整个文件的首个rowkey或者最后一个rowkey的话，就认为没有切分点。如一个文件只有一个block的情况。



### 写入流程

客户端通过发请求到RegionServer端，写入的时候会先写入WAL日志中，其次会将数据写入memstore内存，当数据达到一定大小后会flush成一个个的HFile文件，当文件达到一定数量后，通过compaction的操作合并成更大文件，这样数据读取会更快。



### 读取流程

当进行读取时，客户端会先发送scan请求到RegionServer，打开scanner，然后调next请求获取一行数据，也可以将一批数据先放入Result的Cache中去，客户端不断迭代获取内容。

#### BlockCache

读取的时候，先去MemStore中获取，再去BlockCache，最后到HFile中。

BlockCache 中的Block 是HBase从硬盘完成一次读取的数据单位。HFile 物理存放形式是一个Block 的序列外加这些Block的索引。

LRUBlock Cache：基于JVM Heap的Cache；

Bucket Cache：基于堆外内存，二级索引机制，一级索引是LRUBlockCache（存放Index Block 和 Bloom Block）；

`-XX:MaxDirectMemorySize`：定义JVM可以获得的堆外内存上限，如果不配置这个参数，JVM会按需索取堆外内存，这个参数值必须比`hbase.bucketcache.size`大。