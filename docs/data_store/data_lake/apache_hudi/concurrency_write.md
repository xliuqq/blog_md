# 并发控制

多个写入者时数据的摄取（ingest）方式。



## 支持的并发控制

### MVCC

- Hudi 表服务，如压缩、清理、集群利用**多版本并发控制**来提供多个表服务**写入者和读取者之间的快照隔离**。
- Hudi 在摄取写入器和多个并发读取器之间提供快照隔离
- Hudi 支持并发运行任意数量的表服务作业，没有任何并发冲突。这是通过确保此类表服务的调度计划始终以**单一写入模式**发生以确保没有冲突并避免竞争条件而成为可能的。

### OPTIMISTIC CONCURRENCY

写入操作，如上述（UPSERT、INSERT）等，利用乐观并发控制使多个摄取写入器能够写入同一个 Hudi 表。

Hudi 支持`file level OCC`，即对于发生在同一个表上的任何 2 个提交（或写入者），如果它们**没有写入正在更改的重叠文件**，则允许两个写入者成功。

此功能目前处于***实验阶段***，需要 Zookeeper 或 HiveMetastore 来获取锁。



## 单写保证

- *UPSERT*：目标表不会展示重复记录；
- *INSERT* ：目标表不会有重复记录，如果`hoodie.datasource.write.insert.drop.duplicates`设置为`true`；
- *BULK_INSERT* ：目标表不会有重复记录，如果`hoodie.datasource.write.insert.drop.duplicates`设置为`true`；
- *INCREMENTAL PULL*：数据消费和检查点永远不会出现故障。



## 多写保证

- *UPSERT*：目标表不会展示重复；
- *INSERT* ：目标表**可能有重复**记录，如果`hoodie.datasource.write.insert.drop.duplicates`设置为`true`；
- *BULK_INSERT* ：目标表**可能有重复**记录，如果`hoodie.datasource.write.insert.drop.duplicates`设置为`true`；
- *INCREMENTAL PULL*：数据消费和检查点**可能会出现故障**，由于多个写作业在不同时间完成。



## 允许多写

### 设置

```properties
hoodie.write.concurrency.mode=optimistic_concurrency_control
hoodie.cleaner.policy.failed.writes=LAZY
hoodie.write.lock.provider=<lock-provider-classname>
```

#### Zookeeper锁

```properties
hoodie.write.lock.provider=org.apache.hudi.client.transaction.lock.ZookeeperBasedLockProvider
hoodie.write.lock.zookeeper.url
hoodie.write.lock.zookeeper.port
hoodie.write.lock.zookeeper.lock_key
hoodie.write.lock.zookeeper.base_path
```

#### HiveMetaStore锁

```properties
hoodie.write.lock.provider=org.apache.hudi.hive.HiveMetastoreBasedLockProvider
hoodie.write.lock.hivemetastore.database
hoodie.write.lock.hivemetastore.table
```



### Spark DataSource Write

`hudi-spark`提供DataSource API写入和读取Hudi表为DataFrame。

乐观并发控制示例：

```scala
inputDF.write.format("hudi")
       .options(getQuickstartWriteConfigs)
       .option(PRECOMBINE_FIELD_OPT_KEY, "ts")
       .option("hoodie.cleaner.policy.failed.writes", "LAZY")
       .option("hoodie.write.concurrency.mode", "optimistic_concurrency_control")
       .option("hoodie.write.lock.zookeeper.url", "zookeeper")
       .option("hoodie.write.lock.zookeeper.port", "2181")
       .option("hoodie.write.lock.zookeeper.lock_key", "test_table")
       .option("hoodie.write.lock.zookeeper.base_path", "/test")
       .option(RECORDKEY_FIELD_OPT_KEY, "uuid")
       .option(PARTITIONPATH_FIELD_OPT_KEY, "partitionpath")
       .option(TABLE_NAME, tableName)
       .mode(Overwrite)
       .save(basePath)
```

### DeltaStreamer

乐观并发控制示例：**将相关配置加到`kafka-source.properties`文件中，并作为props传入**

```shell
$ spark-submit --class org.apache.hudi.utilities.deltastreamer.HoodieDeltaStreamer `ls packaging/hudi-utilities-bundle/target/hudi-utilities-bundle-*.jar` \
  --props file://${PWD}/hudi-utilities/src/test/resources/delta-streamer-config/kafka-source.properties \
  --schemaprovider-class org.apache.hudi.utilities.schema.SchemaRegistryProvider \
  --source-class org.apache.hudi.utilities.sources.AvroKafkaSource \
  --source-ordering-field impresssiontime \
  --target-base-path file:\/\/\/tmp/hudi-deltastreamer-op \ 
  --target-table uber.impressions \
  --op BULK_INSERT
```

## 禁止多写

默认配置，以下参数

```properties
hoodie.write.concurrency.mode=single_writer
hoodie.cleaner.policy.failed.writes=EAGER
```



## 乐观并发控制最佳实践

配置重试以允许您的应用程序获取锁：

- 网络连接或服务器负载过大增加了获取锁的时间，从而导致超时
- 运行大量并发作业写入同一个 hudi 表可能会导致锁获取期间的争用可能导致超时
- 在某些冲突解决方案中，Hudi 提交操作可能需要长达 10 秒的时间，同时持有锁。这可能导致等待获取锁的其他作业超时。

 lock provider client retries

```properties
hoodie.write.lock.wait_time_ms_between_retry
hoodie.write.lock.num_retries
```

hudi client端重试的配置

```properties
hoodie.write.lock.client.wait_time_ms_between_retry
hoodie.write.lock.client.num_retries
```

