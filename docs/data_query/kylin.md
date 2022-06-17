# Apache Kylin

​	 Kylin 是一个分布式的大数据分析引擎，提供在 Hadoop 之上的 **SQL 接口和多维分析能力（OLAP）**，可以做到在 TB 级的数据量上实现亚秒级的查询响应。 

<img src="pics/kylin_diagram.png" alt="kylin_diagram" style="zoom:75%;" />

​	Kylin 利用 MapReduce/Spark 将原始数据进行聚合计算，转成了 OLAP Cube 并加载到 HBase 中，以 Key-Value 的形式存储。 Cube 按照时间范围划分为多个 segment，每个 segment 是一张 HBase 表 