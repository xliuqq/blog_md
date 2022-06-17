# Spark

[TOC]

## Spark基本原理、构架和编程技术

- 为什么会有Spark？
- Spark主要体系结构和组件？
- Spark集群的主要构成？
- Spark应用程序的主要结构？
- 。。。

## Spark基本概念

| **Term**        | **Meaning**                                                  |
| --------------- | ------------------------------------------------------------ |
| Application     | User program built on Spark. Consists of  a *driver program* and *executors* on the cluster. |
| Application jar | A jar containing the user's Spark  application. In some cases users will want to create an "uber jar"  containing their application along with its dependencies. The user's jar  should never include Hadoop or Spark libraries, however, these will be added  at runtime. |
| Driver program  | The process running the main() function of  the application and creating the SparkContext |
| Cluster manager | An external service for acquiring resources  on the cluster (e.g. standalone manager, Mesos, YARN) |
| Deploy mode     | Distinguishes where the driver process runs.  In "cluster" mode, the framework launches the driver inside of the  cluster. In "client" mode, the submitter launches the driver  outside of the cluster. |
| Worker node     | Any node that can run application code in the  cluster       |
| Executor        | A process launched for an application on a  worker node, that runs tasks and keeps data in memory or disk storage across  them. Each application has its own executors. |
| Task            | A unit of work that will be sent to one  executor            |
| Job             | A parallel computation consisting of  multiple tasks that gets spawned in response to a Spark action (e.g. save, collect); you'll see this term used in the driver's logs. |
| Stage           | Each job gets divided into smaller sets of  tasks called *stages* that  depend on each other (similar to the map and reduce stages in MapReduce);  you'll see this term used in the driver's logs. |



## Spark内部原理

- [Shuffle的几种方式及其区别](spark_shuffle.md)
- [序列化方式及其使用场景](spark_serializer.md)
- 函数的闭包清理

## Spark性能调优

- 对比kryo和java原生的序列化性能？
- Spark Web UI的查看，及指标分析？
- 数据倾斜，怎么解决？
- 并发度怎么配置？
- GC信息怎么查看，各个字段代表什么意义，如何优化GC性能？

## Spark代码实战

- 入门级：word count算法并行化实现和性能分析？
- 进阶级：Kmeans算法并行化实现和性能分析？
- 毕业级：FDTD3d的代码并行化实现和性能分析？



Spark SQL

Spark Graph

Spark Streaming

Spark MLlib

## 参考资料

### [书籍_Spark Definitive_Guide_2018](materials/Bill Chambers_Matei Zaharia-Spark_ The Definitive Guide_ Big Data Processing Made Simple_2018.pdf)

### [PPT_Spark原理、架构、编程](materials/大数据内存计算系统Spark的基本原理、构架与编程技术.pdf)