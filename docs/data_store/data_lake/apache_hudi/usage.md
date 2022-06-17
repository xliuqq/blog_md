# 使用







## 参数

- `hoodie.table.precombine.field`：在数据合并的时候使用到，当 `hoodie.table.recordkey.fields`相同时，默认取`hoodie.table.precombine.field`属性配置的字段最大值所对应的行；

- `hoodie.table.recordkey.fields`：每条记录的唯一id，支持多个字段；

- `hoodie.table.partition.fields`：用于存放数据的分区字段；



**注：（`_hoodie_commit_time`必须是当前时间，不能是event time）**



记录有效负载定义如何根据传入的新记录和存储的旧记录生成新值。

- `hoodie.payload.event.time.field`：推断与记录关联的时间戳的列/字段名，默认为`ts`，用于获取event time信息，写入commit_time。



`BEGIN_INSTANTTIME_OPT_KEY`

OPERATION_OPT_KEY





### Copy-On-Write tables

| Query Engine         | Snapshot Queries | Incremental Queries |
| -------------------- | ---------------- | ------------------- |
| **Hive**             | Y                | Y                   |
| **Spark SQL**        | Y                | Y                   |
| **Spark Datasource** | Y                | Y                   |
| **Flink SQL**        | Y                | N                   |
| **PrestoDB**         | Y                | N                   |
| **Impala**           | Y                | N                   |

### Merge-On-Read tables

| Query Engine         | Snapshot Queries | Incremental Queries | Read Optimized Queries |
| -------------------- | ---------------- | ------------------- | ---------------------- |
| **Hive**             | Y                | Y                   | Y                      |
| **Spark SQL**        | **Y**            | **Y**               | **Y**                  |
| **Spark Datasource** | **Y**            | **Y**               | **Y**                  |
| **Flink SQL**        | Y                | Y                   | Y                      |
| **PrestoDB**         | Y                | N                   | Y                      |
| **Impala**           | N                | N                   | Y                      |



## Flink

TODO：Insert SQL 语句是流式的不停止？



## Spark







