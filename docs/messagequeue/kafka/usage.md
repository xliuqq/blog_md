## kafka命令

启动

```
cd /home/experiment/workspace/kafka_2.11-2.2.1/bin
nohup ./kafka-server-start.sh ../config/server.properties 2>&1 &
```

关闭

```
cd /home/experiment/workspace/kafka_2.11-2.2.1/bin
./kafka-server-stop.sh
```

创建topic

```
./kafka-topics.sh \
--create \
--zookeeper 172.16.2.243:2181,172.16.2.244:2181,172.16.2.245:2181 \
--replication-factor 2 \
--partitions 6 \
--topic test_hudi
```

查看topic命令

```
cd /home/experiment/workspace/kafka_2.11-2.2.1/bin
./kafka-topics.sh \
--list \
--zookeeper 172.16.2.243:2181,172.16.2.244:2181,172.16.2.245:2181
```

生产者生产数据

```
./kafka-console-producer.sh \
--bootstrap-server 172.16.2.243:9092,172.16.2.244:9092,172.16.2.245:9092 \
--topic test_hudi
```

消费者消费数据

```
./kafka-console-consumer.sh \
--bootstrap-server 172.16.2.244:9092 \
--from-beginning \
--topic test_hudi
```

查看topic描述信息

```
cd /home/experiment/workspace/kafka_2.11-2.2.1/bin
./kafka-topics.sh \
--describe \
--zookeeper master01:2181,slave02:2181,slave04:2181,slave05:2181,slave06:2181,slave07:2181 \
--topic train_realtime_status
```

增加topic分区数

```
cd /home/experiment/workspace/kafka_2.11-2.2.1/bin
./kafka-topics.sh \
--zookeeper master01:2181,slave02:2181,slave04:2181,slave05:2181,slave06:2181,slave07:2181 \
--alter \
--topic train_realtime_status \
--partitions 8
```

增加配置

```
cd /home/experiment/workspace/kafka_2.11-2.2.1/bin
./kafka-topics.sh \
--zookeeper master01:2181,slave02:2181,slave04:2181,slave05:2181,slave06:2181,slave07:2181 \
--alter \
--topic train_realtime_status \
--config flush.messages=10pwd
```

删除配置

```
cd /home/experiment/workspace/kafka_2.11-2.2.1/bin
./kafka-topics.sh \
--zookeeper master01:2181,slave02:2181,slave04:2181,slave05:2181,slave06:2181,slave07:2181 \
--alter \
--topic train_realtime_status \
--delete-config flush.messages
```

删除topic

```
./kafka-topics.sh \
--zookeeper 172.16.2.243:2181,172.16.2.244:2181,172.16.2.245:2181 \
--delete \
--topic test_hudi
```