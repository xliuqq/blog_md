# [Delta Lake](https://delta.io/)

> Parquet文件 + Meta 文件 + 一组操作的API = Delta Lake.

## 特性

1. 元数据也是大数据，设计了基于分布式存储（如HDFS）的元数据系统，解决metastore不堪重负的问题；
2. 支持更多种类的更新模式，比如Merge/Update/Delete等操作，配合流式写入或者读取的支持，让实时数据湖变得水到渠成；
3. 流批操作可以共享同一张表；
4. 版本概念，可以随时回溯，避免一次误操作或者代码逻辑而无法恢复的灾难性后果。



## 场景

Delta Lake is an open-source storage layer that brings ACID transactions to Apache Spark™ and big data workloads。（ Linux 基金会）

![image-20200327092651769](C:\Users\xliu\AppData\Roaming\Typora\typora-user-images\image-20200327092651769.png)

Spark 使用

```scala
dataframe.write.format("delta").save("/data")
```



