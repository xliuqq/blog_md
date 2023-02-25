# 历史

数据湖场景下的元数据管理系统 Hudi Metastore：

- Hive Metastore 基于目录的元数据管理方式太粗，没有办法满足数据湖以 commit 的形式管理元数据的需求

并发写入，并且支持灵活的行列冲突检查策略



## 0.12.0

- PrestoDB
  - 0.275 版本开始，用户现在可以利用原生 Hudi 连接器来查询 Hudi 表
- HUDI-4500：Support restore to savepoint with holes in the timeline
- 乐观并发控制的多个写入器支持基于文件系统的锁；
- Flink INSERT 操作支持异步Clustering；

## 0.11.0

- 支持多级索引
- Bucket索引：
  - **Bucket Pruning：**利用表的 Bucket 分布对读取数据进行剪枝。
  - **Bucket Join：**利用表的 Bucket 分布减少 Join/Aggregate 带来的 shuffle 操作
- COW表的加密：Spark 3.2 + Parquet 1.2的特性

- **添加CALL命令以支持在 Hudi 表上调用更多操作**
- 针对Spark 3.1、Spark 3.2版本增加了schema功能的演进





## 0.10.0 



## 0.9.0



### 通用

- （？）Support streaming read with compaction and cleaning
- 增强 Bulk_Insert模式（新增行写入器模式），并缺省打开，用户可以使用行写入器模式以获得更好的性能。



### Flink

- 写入支持 `CDC Format`的 MOR 表；
- 支持Bulk insert来加载现有表；
- 流式读取 COW 表；
- 写入现在可以更新历史分区（`index.global.enabled`）；
- 支持不同的 Hive 版本（1.x、2.x、3.x），改善了 Hive 同步；
- 支持纯日志追加模式，没有记录去重，直接写Parquet（关闭 `write.insert.deduplicate`）；



### Spark

- 添加了对使用 Spark SQL 的 DDL/DML 的支持；

- SQL 支持时间旅行（Time travel）；

- Hudi 现在可以在 Spark 中注册为数据源表；

- 基于Metadata Table的 Spark 读取改进；

- Clusetering

  - DeltaStreamer 和 Spark Streaming 都添加了异步Clustering支持；

  - 增量读取也适用于Clustering数据；

  - 添加了 HoodieClusteringJob[6] 以作为独立作业来构建和执行Clustering计划；


- Spark 引擎中添加了预提交验证器框架；
- DeltaStreamer
  - 对`hudi-cli` 的一些改进，例如`SCHEDULE COMPACTION`和`RUN COMPACTION`语句，以便轻松在 Hudi 表上调度和运行Compaction、Clustering。