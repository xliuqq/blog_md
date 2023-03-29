# Spark SQL

http://spark.apache.org/docs/latest/sql-programming-guide.html



## DataFrame

Koalas：基于 Spark DataFrame 实现的分布式 Pandas DataFrame，已经被集成到 Spark 3.x 中。



## SQL语法

### 辅助语句

#### refresh

`REFRESH` is used to invalidate and refresh all the cached data (and the associated metadata) for all Datasets that contains the given data source path. Path matching is by prefix, i.e. “/” would invalidate everything that is cached.

```sql
df = spark.read.format("hudi").load(basePath+"/*/*")
df.createOrReplaceTempView("track_log")
# 刷新表的元数据和data
spark.sql("refresh table mytable")
```



## Hive

[Spark的Hive版本的管理](https://spark.apache.org/docs/latest/sql-data-sources-hive-tables.html)




