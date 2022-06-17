# Spark kafka使用

依赖

```
groupId = org.apache.spark
artifactId = spark-sql-kafka-0-10_2.12
version = 3.1.2
```



```scala
// Subscribe to 1 topic, with headers
val df = spark
  .readStream
  .format("kafka")
  .option("kafka.bootstrap.servers", "host1:port1,host2:port2")
  .option("subscribe", "topic1,topic2")   // 订阅多个主题，逗号隔开
  .option("includeHeaders", "true")
  .load()
df.selectExpr("CAST(key AS STRING)", "CAST(value AS STRING)", "headers")
  .as[(String, String, Array[(String, Array[Byte])])]
```



