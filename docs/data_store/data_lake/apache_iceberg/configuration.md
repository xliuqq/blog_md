[toc]

# 配置

## 表的配置

### 读配置

| Property                        | Default            | Description                                                  |
| :------------------------------ | :----------------- | :----------------------------------------------------------- |
| read.split.target-size          | 134217728 (128 MB) | Target size when combining data input splits                 |
| read.split.metadata-target-size | 33554432 (32 MB)   | Target size when combining metadata input splits             |
| read.split.planning-lookback    | 10                 | Number of bins to consider when combining input splits       |
| read.split.open-file-cost       | 4194304 (4 MB)     | The estimated cost to open a file, used as a minimum weight when combining splits. |

### 写配置

| Property                                   | Default            | Description                                                  |
| :----------------------------------------- | :----------------- | :----------------------------------------------------------- |
| write.format.default                       | parquet            | Default file format for the table; parquet, avro, or orc     |
| write.parquet.row-group-size-bytes         | 134217728 (128 MB) | Parquet row group size                                       |
| write.parquet.page-size-bytes              | 1048576 (1 MB)     | Parquet page size                                            |
| write.parquet.dict-size-bytes              | 2097152 (2 MB)     | Parquet dictionary page size                                 |
| write.parquet.compression-codec            | gzip               | Parquet compression codec                                    |
| write.parquet.compression-level            | null               | Parquet compression level                                    |
| write.avro.compression-codec               | gzip               | Avro compression codec                                       |
| write.location-provider.impl               | null               | Optional custom implemention for LocationProvider            |
| write.metadata.compression-codec           | none               | Metadata compression codec; none or gzip                     |
| write.metadata.metrics.default             | truncate(16)       | Default metrics mode for all columns in the table; none, counts, truncate(length), or full |
| write.metadata.metrics.column.col1         | (not set)          | Metrics mode for column ‘col1’ to allow per-column tuning; none, counts, truncate(length), or full |
| write.target-file-size-bytes               | Long.MAX_VALUE     | Controls the size of files generated to target about this many bytes |
| write.distribution-mode                    | none               | Defines distribution of write data: **none**: don’t shuffle rows; **hash**: hash distribute by partition key ; **range**: range distribute by partition key or sort key if table has an SortOrder |
| write.wap.enabled                          | false              | Enables write-audit-publish writes                           |
| write.summary.partition-limit              | 0                  | Includes partition-level summary stats in snapshot summaries if the changed partition count is less than this limit |
| write.metadata.delete-after-commit.enabled | false              | Controls whether to delete the oldest version metadata files after commit |
| write.metadata.previous-versions-max       | 100                | The max number of previous version metadata files to keep before deleting after commit |
| write.spark.fanout.enabled                 | false              | Enables Partitioned-Fanout-Writer writes in Spark            |

### 表的表现配置

| Property                             | Default            | Description                                                  |
| :----------------------------------- | :----------------- | :----------------------------------------------------------- |
| commit.retry.num-retries             | 4                  | Number of times to retry a commit before failing             |
| commit.retry.min-wait-ms             | 100                | Minimum time in milliseconds to wait before retrying a commit |
| commit.retry.max-wait-ms             | 60000 (1 min)      | Maximum time in milliseconds to wait before retrying a commit |
| commit.retry.total-timeout-ms        | 1800000 (30 min)   | Maximum time in milliseconds to wait before retrying a commit |
| commit.manifest.target-size-bytes    | 8388608 (8 MB)     | Target size when merging manifest files                      |
| commit.manifest.min-count-to-merge   | 100                | Minimum number of manifests to accumulate before merging     |
| commit.manifest-merge.enabled        | true               | Controls whether to automatically merge manifests on writes  |
| history.expire.max-snapshot-age-ms   | 432000000 (5 days) | Default max age of snapshots to keep while expiring snapshots |
| history.expire.min-snapshots-to-keep | 1                  | Default min number of snapshots to keep while expiring snapshots |

### 兼容性配置

| Property                                      | Default | Description                                                |
| :-------------------------------------------- | :------ | :--------------------------------------------------------- |
| compatibility.snapshot-id-inheritance.enabled | false   | Enables committing snapshots without explicit snapshot IDs |



## Catalog配置

`HadoopCatalog` and `HiveCatalog` can access the properties in their constructors. 

| Property     | Default | Description                                           |
| :----------- | :------ | :---------------------------------------------------- |
| catalog-impl | null    | a custom `Catalog` implementation to use by an engine |
| io-impl      | null    | a custom `FileIO` implementation to use in a catalog  |
| warehouse    | null    | the root path of the data warehouse                   |
| uri          | null    | a URI string, such as Hive metastore URI              |
| clients      | 2       | client pool size                                      |

### Lock catalog 配置

一些Catalog实现使用这些参数来控制提交期间的锁行为。

| Property                   | Default    | Description                                                  |
| :------------------------- | :--------- | :----------------------------------------------------------- |
| lock-impl                  | null       | a custom implementation of the lock manager, the actual interface depends on the catalog used |
| lock.table                 | null       | an auxiliary table for locking, such as in [AWS DynamoDB lock manager](http://iceberg.apache.org/aws/#dynamodb-for-commit-locking) |
| lock.acquire-interval-ms   | 5 seconds  | the interval to wait between each attempt to acquire a lock  |
| lock.acquire-timeout-ms    | 3 minutes  | the maximum time to try acquiring a lock                     |
| lock.heartbeat-interval-ms | 3 seconds  | the interval to wait between each heartbeat after acquiring a lock |
| lock.heartbeat-timeout-ms  | 15 seconds | the maximum time without a heartbeat to consider a lock expired |



## Hadoop 配置

使用Hive元数据连接时，可以使用以下配置：

| Property                            | Default        | Description                                                  |
| :---------------------------------- | :------------- | :----------------------------------------------------------- |
| iceberg.hive.client-pool-size       | 5              | The size of the Hive client pool when tracking tables in HMS |
| iceberg.hive.lock-timeout-ms        | 180000 (3 min) | Maximum time in milliseconds to acquire a lock               |
| iceberg.hive.lock-check-min-wait-ms | 50             | Minimum time in milliseconds to check back on the status of lock acquisition |
| iceberg.hive.lock-check-max-wait-ms | 5000           | Maximum time in milliseconds to check back on the status of lock acquisition |

Note: `iceberg.hive.lock-check-max-wait-ms` should be less than the [transaction timeout](https://cwiki.apache.org/confluence/display/Hive/Configuration+Properties#ConfigurationProperties-hive.txn.timeout) of the Hive Metastore (`hive.txn.timeout` or `metastore.txn.timeout` in the newer versions). Otherwise, the heartbeats on the lock (which happens during the lock checks) would end up expiring in the Hive Metastore before the lock is retried from Iceberg.