# Kafka

> 分布式流处理平台

流处理平台具有三个关键能力：

1. 发布和订阅消息(流)，在这方面，它类似于一个消息队列或企业消息系统。
2. 以容错(故障转移)的方式存储消息(流)。
3. 在消息流发生时处理它们。

## 场景

1. 构建实时的流数据管道，可靠地获取系统和应用程序之间的数据。
2. 构建实时流的应用程序，对数据流进行转换或反应。



## 术语

#### Topic

Kafka将消息分门别类，**每一类的消息称之为一个主题**（Topic）。

- Topic/Partition 数量过多，导致顺序写退化成随机写；
  - 每个 Partition 都是一个目录；

#### Producer

**发布消息**的对象称之为主题生产者（Kafka topic producer）。

#### Consumer

**订阅消息**并处理发布的消息的对象称之为主题消费者（consumers）。

Consumer Group（消费组）的概念，多个 Consumer 组团去消费一个 Topic：

- 同组的 Consumer 有相同的 Group ID；
- Consumer Group 机制会保障一条消息**只被组内唯一一个 Consumer 消费**，不会重复消费。

#### Broker

已发布的消息保存在一组服务器中，称之为Kafka集群。集群中的每一个服务器都是一个代理（Broker）。 消费者可以订阅一个或多个主题（topic），并从Broker拉数据，从而消费这些已发布的消息。

#### Partition

produce发送的消息分发到不同的partition中，consumer接受数据的时候是按照group来接受：

- 在发送一条消息时，可以指定这条消息的key，Producer根据这个**key和Partition机制**来判断应该将这条消息发送到哪个Parition。
- kafka确保每个**partition只能同一个group中的同一个consumer消费**，如果想要重复消费，那么需要其他的组来消费。



Topic的Partitions分布在不同的Broker上

- 单个 Broker 的 IO能力受限，Partition分散可扩展性强；
- Consumer 的多个实例连接不同 broker，消费不同分区数据；

## 架构

![kafka使用](pics/kafka_flow.png)



## 设计

### 日志Log

对于每个topic，Kafka集群都会维护一个分区log，就像下图中所示：

![screenshot](pics/kafka_topics.png)

每一个分区都是一个顺序的、不可变的消息队列， 并且可以持续的添加。分区中的消息都被分了一个序列号，称之为偏移量(offset)，在每个分区中此偏移量都是唯一的。



### 存储

kafka 写数据：

- 基于 OS 层面的 Page Cache 来写数据；
- 采用磁盘顺序写的方式；

通过零拷贝技术，就不需要把 OS Cache 里的数据拷贝到应用缓存，再从应用缓存拷贝到 Socket 缓存了，两次拷贝都省略了，所以叫做零拷贝。



### 分区

![img](pics/kafka_partition.jpg)

### 写

分区选择的三种方式：

1）写入时，直接指定分区；

2）根据 Partition Key 写入特定 Partition，保证此类消息的有序性

- 由特定 Hash 函数，决定写入哪个 Partition，有相同 Partition Key 的消息，会被放到相同的 Partition；
- 热点问题，某个 Partition 特别繁忙；

3）不指定 Key，Kafka 采用轮询的方式写入，分散均匀但不保证有序性；

4）自定义规则，Producer 可以使用自己的分区指定规则；

### 读

分区读取的方式：

- Consumer 必须自己从 Topic 的 Partition 根据 Offset 拉取消息；
- 可以指定消费特定的分区；
- Offset 的推进和记录都是 Consumer 的责任，定期会提交offset 给kafka内部topic：`__consumer_offsets`；
  - 根据 comsumer group 进行区分不同客户端；

