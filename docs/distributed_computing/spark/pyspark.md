# PySpark

## 原理

<img src="./pics/pyspark_arch.png" alt="img" style="zoom: 75%;" />

其中白色部分是新增的Python进程：

- 在Driver端，通过Py4j实现在Python中调用Java的方法，即将用户写的PySpark程序”映射”到JVM中；
  - Python进程通过Py4j调用Java方法提交Job，Job运行结果通过本地Socket被拉取到Python进程
  - 对于大数据量，例如广播变量等，**Python进程和JVM进程是通过本地文件系统来交互**，以减少进程间的数据传输

- 在Executor端，则不需要借助Py4j，因为Executor端运行的Task逻辑是由Driver发过来的，那是序列化后的字节码。
  - 每个Executor上有一个公共的pyspark.deamon进程，负责接收Task请求，并`fork pyspark.worker`进程单独处理每个Task，实际数据处理过程中，pyspark.worker进程和 JVM Task 会较频繁地进行本地Socket数据通信。
  - 数据通过**本地 socket** 协议发送给Python Worker，期间需要将 InternalRow 转化为 Java对象，然后再用 **Java Pickle 进行序列化**(一次)，这个时候才能通过网络发送给Worker。

Python 代码的序列化：

- driver端在调用时将其闭包序列化

## Arrow加速

### 分析

> spark.sql.execution.arrow.pyspark.enabled	默认 false
>
> 一次计算，需要触发两次 Java / Python 的序列化和反序列化。

- Spark 触发计算时（如加载数据），把数据转成内部存储格式 InternalRow，再转化为Java对象，然后再用 **Java Pickle进行序列化**发送给Worker
- Worker接收后，再一条一条**反序列化(python pickle)**转化为Python对象进行处理；
- 处理完成后，再用 **pickle 进行序列化**，发送给Java Executor；
- Java Executor获取数据后，需要**再反序列化**，然后转化为InternalRow继续进行处理。



Arrow 改造：`spark.sql.execution.arrow.maxRecordsPerBatch`（默认10000）

- 分区被转为多个Arrow record batches，会引起临时性的内存高峰；



### FallBack

> spark.sql.execution.arrow.pyspark.fallback.enabled 默认 true

不一定适用所有情况（即支持所有的数据类型），当出错时，会自动回退到未优化的实现。

Currently(V3.4.0), all Spark SQL data types are supported by Arrow-based conversion except [`ArrayType`](https://spark.apache.org/docs/latest/api/python/reference/pyspark.sql/api/pyspark.sql.types.ArrayType.html#pyspark.sql.types.ArrayType) of [`TimestampType`](https://spark.apache.org/docs/latest/api/python/reference/pyspark.sql/api/pyspark.sql.types.TimestampType.html#pyspark.sql.types.TimestampType). 

[`MapType`](https://spark.apache.org/docs/latest/api/python/reference/pyspark.sql/api/pyspark.sql.types.MapType.html#pyspark.sql.types.MapType) and [`ArrayType`](https://spark.apache.org/docs/latest/api/python/reference/pyspark.sql/api/pyspark.sql.types.ArrayType.html#pyspark.sql.types.ArrayType) of nested [`StructType`](https://spark.apache.org/docs/latest/api/python/reference/pyspark.sql/api/pyspark.sql.types.StructType.html#pyspark.sql.types.StructType) are only supported when using PyArrow 2.0.0 and above.
