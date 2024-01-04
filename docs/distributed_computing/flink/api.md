# Flink API

DataSet 和 DataStream的流程：

1. 获取`execution environment`；
2. 加载/创建数据；
3. 数据转换；
4. 结果存储；
5. 触发程序执行；

data source -> data operations -> data sink



Flink的执行通过 Environment 的 execute 方法，是**根据sink（输出）决定输入以及执行的操作**。

- 两次execute之间，新的sink会被输出；

## 交互式编程(Not Done)

https://cwiki.apache.org/confluence/display/FLINK/FLIP-36%3A+Support+Interactive+Programming+in+Flink

DataSet 的 API，可以交互式



## DataSet(scala)

有界数据，DataSet 类似 RDD，提供 filter等转换，以及collect, print等sink；

ExecutionEnvironment

**获取执行环境：**

```java
getExecutionEnvironment()

createLocalEnvironment()

createRemoteEnvironment(String host, int port, String... jarFiles)
```



## DataStream(scala/python)

无界数据

StreamExecutionEnvironment

Flink的执行，通过Environment进行执行。。



## Table(scala/python)