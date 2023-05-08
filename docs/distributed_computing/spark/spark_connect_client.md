# Spark Connect

> In Apache Spark 3.4, Spark Connect introduced a decoupled client-server architecture that allows remote connectivity to Spark clusters using the **DataFrame** API and **unresolved logical plans** as the protocol. 



## 架构

![spark-connect-api](./pics/spark-connect-api.png)



## 原理

Spark Connect客户端将 DataFrame 操作转换为未解析的逻辑查询计划，并用 Protocol Buffers 进行编码，通过 gRPC 发送给 Server。

Spark Server上嵌入的Spark Connect端点接收未解析的逻辑计划并将其转换为Spark的逻辑计划运算符。然后标准的Spark执行过程介入。

结果通过 gRPC 作为 **Apache Arrow 编码的行批处理 流式**传输回客户端。

![Spark Connect communication](./pics/spark-connect-communication.png)

## 使用

### 启动 Spark connect server

```shell
# 通过 spark-submit (client 模式?) 的方式，运行 org.apache.spark.sql.connect.service.SparkConnectServer
./sbin/start-connect-server.sh --packages org.apache.spark:spark-connect_2.12:3.4.0
```



### Client 使用

```python
from pyspark.sql import SparkSession
spark = SparkSession.builder.remote("sc://localhost").getOrCreate()
```

