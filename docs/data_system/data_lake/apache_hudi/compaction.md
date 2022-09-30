#  Compaction

压缩仅适用于**读时合并（MOR）**表类型，并且由压缩策略（默认选择具有最大未压缩日志的文件片）决定选择要进行压缩的文件片。

Merge-On-Read表通过**delta文件，进行同步/异步的合并**，在**写入**（ingest）记录时减少数据延迟。因此，**异步运行压缩而不阻塞写入**是有意义的。



## 实时压缩

**默认异步压缩。**

操作流程：

- **Compaction Scheduling** ：Hudi扫描分区并选择要压缩的file slices，一份压缩计划最终被写进了 Hudi Timeline。

- **Compaction Execution** ：一个单独的进程读取压缩计划并执行file slices的压缩。

### 同步

适用**新提交的立即读取**场景，写程序进程本身在每次写入之后同步执行的，即直到压缩完成后才能开始下一个写操作。

### 异步

压缩过程可以与表的写操作同时异步运行，压缩不会阻塞下一批数据写入，从而产生近实时的数据新鲜度。



## 配置

- `hoodie.compaction.strategy`：决定在每次压缩运行期间选择哪些文件组进行压缩。默认（LogFileSizeBasedCompactionStrategy ），Hudi选择**具有最多未合并数据的日志**文件；
- `hoodie.compact.inline`：**是否在每次commit之后，触发compaction服务**，增加写延迟，**默认false**，异步；
  - 设置为true时，做 schedule 和 execution 两件事；
  - 对应spark参数：`hoodie.datasource.compaction.async.enable`
  - 对应flink参数： `compaction.async.enabled`(flink)

- `hoodie.compact.schedule.inline（？？）`：默认false，是否在**写入时执行compaction中的schedule**；
- `hoodie.compact.inline.trigger.strategy`：默认`NUM_COMMITS`，compaction调度触发的依据；
  - `hoodie.compact.inline.max.delta.seconds`：上次compaction后多长时间，可以开启新的compaction；
  - `hoodie.compact.inline.max.delta.commits`：上次compaction后多少个delta的commit，可以开启新的comapaction；



## 离线压缩

触发策略默认在完成五次提交后执行压缩。因为compaction会消耗大量内存，并且和写操作放在同一个管道中，所以当数据量很大（>100000/s）时很容易干扰写操作。

- Compaction 有两部：生成计划和执行计划，建议**生成计划由写任务周期性触发**；



### Hudi Compactor（Spark）

Hudi 提供工具类，异步执行特定的 Hudi 表的压缩

```shell
		# instant-time 可选，不设置则会执行timeline中最早的的压缩计划
spark-submit --packages org.apache.hudi:hudi-utilities-bundle_2.12:0.11.0 \
--class org.apache.hudi.utilities.HoodieCompactor \
--base-path <base_path> \
--table-name <table_name> \
--schema-file <schema_file> \
--instant-time <compaction_instant>
```

### Hudi CLI

```shell
hudi:trips->compaction run --tableName <table_name> --parallelism <parallelism> --compactionInstant <InstantTime>
```

### Flink Offline Compaction

```shell
./bin/flink run -c org.apache.hudi.sink.compact.HoodieFlinkCompactor lib/hudi-flink-bundle_2.12-0.11.0.jar --path hdfs://xxx:9000/table
```

具体参数见：https://hudi.apache.org/docs/compaction#flink-offline-compaction



## 示例

### Spark（同步/异步）

参数：

- `hoodie.datasource.compaction.async.enable`：Spark的配置，默认为true，表示异步，会设置`hoodie.compact.inline`为false；
  - 见 `DataSourceUtils::createHoodieConfig`的`asyncCompact`和`inlineCompact`


```java
import org.apache.hudi.DataSourceWriteOptions;
import org.apache.hudi.HoodieDataSourceHelpers;
import org.apache.hudi.config.HoodieCompactionConfig;
import org.apache.hudi.config.HoodieWriteConfig;
import org.apache.spark.sql.streaming.OutputMode;
import org.apache.spark.sql.streaming.ProcessingTime;


 DataStreamWriter<Row> writer = streamingInput.writeStream().format("org.apache.hudi")
        .option(DataSourceWriteOptions.OPERATION_OPT_KEY(), operationType)
        .option(DataSourceWriteOptions.TABLE_TYPE_OPT_KEY(), tableType)
        .option(DataSourceWriteOptions.RECORDKEY_FIELD_OPT_KEY(), "_row_key")
        .option(DataSourceWriteOptions.PARTITIONPATH_FIELD_OPT_KEY(), "partition")
        .option(DataSourceWriteOptions.PRECOMBINE_FIELD_OPT_KEY(), "timestamp")
     	// hoodie.compact.inline.max.delta.commits
        .option(HoodieCompactionConfig.INLINE_COMPACT_NUM_DELTA_COMMITS_PROP, "10")
     	// hoodie.datasource.compaction.async.enable
        .option(DataSourceWriteOptions.ASYNC_COMPACT_ENABLE_OPT_KEY(), "true")
        .option(HoodieWriteConfig.TABLE_NAME, tableName).option("checkpointLocation", checkpointLocation)
        .outputMode(OutputMode.Append());
 writer.trigger(new ProcessingTime(30000)).start(tablePath);
```

### Flink（同步/异步）
