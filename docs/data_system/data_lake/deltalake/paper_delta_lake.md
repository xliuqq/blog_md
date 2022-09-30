# Delta Lake: HighPerformance ACID Table Storage over Cloud Object Stores



对于**对象存储系统**，以key-value形式提供，其元数据操作如list代价比较大，因此很难实现ACID事务和高性能；

使用 Transaction log （Parquet格式）提供ACID支持、时间旅行（Time Travel）和快速的元数据操作。



存储关系型数据集的常用方式是用列式文件格式（如Parquet和ORC），将每个表按照某些字段进行分区，一张表用多个对象进行存储；

- 提供很好的Scan性能；
- 多个对象（分区）的更新不是原子性；
- 写的回滚很难实现；
- 百万分区的表的元操作如list耗时；



we maintain information about which **objects are part of a Delta table in an ACID manner**, using a **write-ahead log** that is itself stored in the cloud object store.

- Time travel：
- UPSERT、DELETE和Merge操作：
- 有效的Streaming I/O：
- Caching：
- 数据布局优化：
- 模式演化：

- 审计日志：



standard DBMS management functions usable directly against low-cost object stores.



- 将频繁访问的数据按顺序保持在较近的位置，这通常会导致选择柱状格式；
- 对象规模大，但不能太大。大对象增加更新数据的代价，因为对象是覆盖全写；
- 避免List操作，在可能的情况下使操作请求按字典顺序排列键；



_last_checkpoint文件主要是提供一个最新的快照ID，帮助减少LIST操作的开销。