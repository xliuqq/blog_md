# SQL Compare

## 对比表 2020/8/19

| SQL引擎\指标 | SQL语句        | 适用范围       | 最近版本<br />(Release) | 数据源           | 数据规模 | 备注               |
| ------------ | -------------- | -------------- | ----------------------- | ---------------- | -------- | ------------------ |
| Hive         | HiveQL         | OLAP           |                         | HDFS             |          |                    |
| Spark SQL    |                | OLAP           |                         |                  |          |                    |
| Presto       | ANSI SQL       | OLAP           |                         | Hive/Kafka/RDBMS |          |                    |
| Impala       |                | OLAP           |                         | HDFS/HBase/Kudu  |          |                    |
| HAWQ         | ANSI SQL       | OLAP，事务ACID | 2018/9                  | HDFS             |          |                    |
| Greenplum    | 基于PostgreSQL | OLAP           | 2020/8                  | 自定义           |          |                    |
| Doris        | MySQL          | OLAP           |                         | 自定义           |          |                    |
| ClickHouse   |                | OLAP           |                         | 自定义           |          |                    |
| Kylin        |                | OLAP           |                         |                  |          | 数据预聚合         |
| Phoenix      |                |                | 2019/12                 |                  |          |                    |
| Cassandra    |                | OLAP           |                         |                  |          |                    |
| Ignite       |                | HTAP           | 2020/5                  |                  |          | 分布式内存计算平台 |
| Mongodb      |                |                |                         |                  |          |                    |
| **TiDB**     | MySQL          | HTAP           |                         | 自定义           |          |                    |
| Calcite      |                | OLAP           |                         |                  |          |                    |
| Drill        |                | OLAP           |                         |                  |          |                    |
| Druid        |                | OLAP           |                         | 自定义           |          |                    |
| Gobblin      |                |                |                         |                  |          |                    |



[开源OLAP引擎测评报告](https://zhuanlan.zhihu.com/p/55197560)

采用 TPC-DS

- 生成数据，dsdgen生成，hive-testbench采用mapreduce分布式生成到hdfs中；
- 适配 TPC-DS 的SQL，dsqgen可以生成**建表**和**查询**的SQL，但是要根据数据库进行适配；
  - hive-testbench包含Hive和SparkSQK的99条SQL；
  - impala的99条SQL -- https://github.com/fayson/cdhproject/tree/master/impala-tpcds



## MPP

对于MPP架构来说，因为task和Executor是绑定的，如果某个Executor执行过慢或故障，将会导致**整个集群的性能就会受限于这个故障节点的执行速度**(所谓木桶的短板效应)，所以MPP架构的最大缺陷就是——**短板效应**。另一点，集群中的节点越多，则某个节点出现问题的概率越大，而一旦有节点出现问题，对于MPP架构来说，将导致整个集群性能受限，所以一般实际生产中**MPP架构的集群节点不易过多**。

举个例子来说下两种架构的数据落盘：要实现两个大表的join操作，对于批处理而言，如Spark将会写磁盘三次(第一次写入：表1根据`join key`进行shuffle；第二次写入：表2根据`join key`进行shuffle；第三次写入：Hash表写入磁盘)， 而MPP只需要一次写入(Hash表写入)。**这是因为MPP将mapper和reducer同时运行，而MapReduce将它们分成有依赖关系的tasks(DAG)，task间是顺序执行的，因此必须通过写入中间数据共享内存来解决数据的依赖**。