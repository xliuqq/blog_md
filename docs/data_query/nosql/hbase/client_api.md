

# 使用（API）

## Shell

启动hbase shell

``` shell
./bin/hbase shell
```

相关命令

```
create 'table_name', 'column_family'

list 'table_name'

describe 'table_name'

put 'table_name', 'row_key', cf:a', 'value1'

scan 'table_name'

get 'table_name', 'row_key'

disable 'table_name'

enable 'table_name'

drop 'table_name'
```

统计行数

```
hbase org.apache.hadoop.hbase.mapreduce.RowCounter 'table_name'
```



## Java Client

> HBase 2.0，不再用HTable作为client API，通过ConnectionFactory创建Connection，在getTable；

- 尽量使用批处理更新来减少单独操作同一行的数据；
- **修改操作只保证行级的原子性**；

###  Connection

**在整个应用（进程）的范围内只维护一个共用的Connection** 

- **Connection**对象需要知道如何**找到HMaster、如何在RegionServer上定位Region，以及感知Region的变动**。所以，**Connection**需要同时与HMaster、RegionServer和ZK建立连接。
- 创建**Connection**是重量级的，已经实现了对连接的管理功能，并且它是线程安全的。
- 由**Connection**取得的Table和Admin对象是轻量级的，并且不是线程安全的，所以它们应该即用即弃。

在并发系统中，多个线程每个都去创建一个Connection对象，**HBase Zookeeper的连接被大量占用**，某一个客户端连接Zookeeper的连接数超过了zk配置，大量连接创建的请求被拒绝。这样会增加ZK的压力，也会导致客户端系统性能急剧下降。

### 增/改（PUT）

- HBase 2.0 没有写缓冲区， Table.put和Table.batch是non-writeBuffer
- 原子性操作，`checkAndMutate`

### 查（GET）



### 删（DELETE）



### 批量操作（Batch）

batch

- 不可以将针对同一行的Put和Delete操作放在同一个批量处理请求，因操作的处理顺序可能不同；
- batch请求是同步的，PUT实例不会被放到缓冲区中；



### 扫描（Scan）

Scan，设置起始行、终止行、过滤器，实现条件查询功能

- setCaching（全局或者Scan设置），一次RPC获取多行数据；
- setBatch（全局或者Scan设置），面向列一级的操作，取回多少列；

<font color='red'>*获取数据的时候，会先获取行对应的region信息，所以开始会比较慢？*</font>

### 高级

HBase的表中的**数据分割主要使用列族**而不是列，不同于传统的列式存储数据库。

- 按行键检索，有效减少查询特定行和行范围的时间；设定列族可以有效减少查询的存储文件；
- 用户应将需要查询的维度或信息存储在行键（左对齐索引）中，筛选效率高（即高表>宽表）；

TODO：图9-2



## Thrift

> The HBase Thrift interface allows other languages to access HBase over Thrift by **connecting to a Thrift server that interfaces with the Java client**.



![img](pics/thrift-hbase-arch)

Thrift API 貌似不可以修改 HBase的client的配置信息，而 Java 的客户端可以修改。
