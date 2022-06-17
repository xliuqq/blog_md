# Apache Kudu

> Kudu的定位是 「Fast Analytics on Fast Data」，是一个既**支持随机读写、又支持 OLAP 分析**的大数据列式存储引擎。



## 架构

Kudu是个存储引擎，不仅仅是存储格式，是个**实时存储系统**，支持毫秒级单行访问。

- NoSQL访问（类似HBase），支持C++、Java、Python API；
- OLAP分析
  - Impala 集成；
  - Hive MetaStore 集成；



**Kudu分为Tablet Server和releaseArtifacts，采用raft通信，不需要Zookeeper。**



### 数据模型

表：

- 主键：允许复合主键；
- 列式存储，列个树不要超过300；
- 



## 存储

数据自行管理，没有采用HDFS。