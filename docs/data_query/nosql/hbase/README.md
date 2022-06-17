# HBase

>  [Hadoop](https://hadoop.apache.org/) database, a distributed, scalable, big data store.
>
> Billion rows with millions columns.



HBase 2.0版本支持将不同的表，底层存储在不同的介质上（HDFS的分层存储）；

## 场景

### 特性

- 容量巨大：单表支持千亿行、百万列，达TB甚至PB级别；
- 可扩展性：数据可扩展性（HDFS），读写服务可扩展性（RegionServer），存算分离；
- 稀疏性：允许列值为空，不进行存储；
- 高性能：OLAP场景，写操作性能好，随机读以及小范围扫描，性能也有保证；
- 多版本：时间戳实现多版本数据，选择最新或者历史版本数据；
- 过期：支持数据TTL特性；
- Hadoop原生支持：
  - 直接操作HDFS文件，实现数据扫描或者数据导入；
  - HBase数据分级存储，重要业务或最近数据放SSD，不重要业务或归档数据放HDD；

### 缺点

- HBase本身不支持复杂的聚合运算（如Join、GroupBy等），可以通过HBase+Phoenix（小规模聚合OLTP）或单独的Spark（大规模OLAP）进行；
- HBase本身没有二级索引功能，可以通过 Phoenix 实现；
- HBase原生不支持全局的跨行事务，只支持单行事务模型，可以通过 Phoenix 实现。

## 数据模型

### 概念

### 物理视图

### 名空间

### Table

表，一张表包含多行数据。

### Row

行，一行数据包含唯一标识RowKey、多个column和对应的值。不同行按**Rowkey的字典序从小到大排序**。

### Column

列，由列簇（column family）和列名（qualifier）组成，通过":"连接。

- 列簇是建表时指定，用户不能随意增减；但列名（qualifier）可以动态扩充。
- 每个列簇的数据集中存放在一个存储单元Store中，建议将具有相同特性的数据设置在同一个列簇中。
- **列簇内是行存，列簇间是列存**。

### Timestamp

时间戳，每个cell写入HBase的时候会默认分配时间戳作为版本信息，时间戳也可以用户写入时指定。

通过时间戳，支持多版本特性，同一个rowkey、column下有多个value存在，版本号越大，数据越新。

### Cell

单元格，由（row，column，timestamp，type，value）组成的结构，type表示是PUT或DELETE。

- 稀疏特性。

KeyValue：一些Key相同的任意数量的独立KeyValue就可以构成一行数据，**每一个KeyValue所携带的自我描述信息，会带来显著的数据膨胀**。（<font color="red">存储的时候会有压缩么？</font>）



## 架构

### Zookeeper

- **Master高可用**：通过ZK分布式锁，选举出Master；
- **核心元数据**：RegionServer集合，元数据表`hbase:meta`所在的RegionServer集合；
- **RegionServer宕机恢复**：ZK通过心跳感知RegionServer存活，宕机后通知Master进行处理；
- **分布式表锁**：HBase中对表进行管理操作时，需要锁表，防止其他用户同时进行表管理操作。

### Master

- **表管理请求处理**：如建表、修改表、权限操作、切分表、合并数据分片以及Compaction等；
- **管理RegionSever**：如RegionServer的负载均衡、RegionServer的宕机恢复和Region的迁移；
- **清理工作**：清理过期日志和文件，如HLog是否过期、HFile是否已经被删除，并在过期之后进行删除。

### RegionServer

- **WAL（HLog）**：Write-Ahead Log，
  - **数据高可靠性**：随机写时，**数据先顺序写入HLog（防止缓存数据丢失）**，再写入缓存，再异步刷新落盘。
  - **集群间主从复制**：通过回放主集群推送过来的HLog日志实现主从复制。
- **BlockCache**：读缓存，时间局部性和空间局部性原理，由`LRUBlockCache`和`BucketCache`两种。
- **Region**：数据表的一个分片，是集群负载聚恒的基本单位。Region由Store组成，一个列簇对应一个Store：
  - **Store**：Store由一个MemStore和多个HFile组成：
    - **MemStore**：写缓存，超过阈值时，异步将数据flush成HFile文件；
    - **HFile**：磁盘数据文件，HFile文件数过多时，会触发Compact操作，多个文件合成一个文件。

### HDFS

- 底层数据存储系统，通过HDFS保证数据（HFile/HLog）的高可用性。

## Data Model Operations

## Versions

## Sort Order

## Column Metadata

## Joins

## ACID