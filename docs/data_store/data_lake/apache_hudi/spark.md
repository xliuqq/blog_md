# Spark Hudi 使用

> 参考官方文档：
>
> - https://hudi.apache.org/docs/quick-start-guide
> - 

- 支持Streaming write；

- `spark.sql.catalog.spark_catalog`：Spark 3.2，`org.apache.spark.sql.hudi.catalog.HoodieCatalog`

## 快速上手

### 准备（生成数据）

spark-shell

```shell
# Spark 3.2
spark-shell \
  --packages org.apache.hudi:hudi-spark3.2-bundle_2.12:0.11.0 \
  --conf 'spark.serializer=org.apache.spark.serializer.KryoSerializer' \
  --conf 'spark.sql.catalog.spark_catalog=org.apache.spark.sql.hudi.catalog.HoodieCatalog'
```

设置信息，DataGenerator是Hudi的工具类，用于生成测试数据

```scala
// spark-shell
import org.apache.hudi.QuickstartUtils._
import scala.collection.JavaConversions._
import org.apache.spark.sql.SaveMode._
import org.apache.hudi.DataSourceReadOptions._
import org.apache.hudi.DataSourceWriteOptions._
import org.apache.hudi.config.HoodieWriteConfig._

val tableName = "hudi_trips_cow"
val basePath = "file:///tmp/hudi_trips_cow"
val dataGen = new DataGenerator
```

### 建表

#### Spark SQL

**新建表**

```sql
-- 创建分区表，设置hudi表的属性
create table hudi_cow_pt_tbl (
  id bigint,
  name string,
  ts bigint,
  dt string,
  hh string
) using hudi
tblproperties (
  type = 'cow',
  primaryKey = 'id',
  preCombineField = 'ts'
 )
partitioned by (dt, hh) location '/tmp/hudi/hudi_cow_pt_tbl';
```

**加载已存在的Hudi表**

```sql
-- 加载分区表，schema和属性，Hudi会自动解析
create table hudi_existing_tbl1 using hudi
partitioned by (dt, hh) location 'file:///tmp/hudi/dataframe_hudi_pt_table';
```

### 插入数据

#### DataFrame

```scala
df.write.format("hudi").
  options(getQuickstartWriteConfigs).
  option(PRECOMBINE_FIELD_OPT_KEY, "ts").
  option(RECORDKEY_FIELD_OPT_KEY, "uuid").
  option(PARTITIONPATH_FIELD_OPT_KEY, "partitionpath").
  option(TABLE_NAME, "hudi_test").
  mode(Overwrite).
  save(basePath)  // file:///tmp/hudi_trips_cow
```

#### SQL

```sql
-- insert into non-partitioned table
insert into hudi_cow_nonpcf_tbl select 1, 'a1', 20;
insert into hudi_mor_tbl select 1, 'a1', 20, 1000;

-- insert dynamic partition
insert into hudi_cow_pt_tbl partition (dt, hh)
select 1 as id, 'a1' as name, 1000 as ts, '2021-12-09' as dt, '10' as hh;

-- insert static partition
insert into hudi_cow_pt_tbl partition(dt = '2021-12-09', hh='11') select 2, 'a2', 1000;
```

### 覆盖写

#### Dataframe

```scala
df.write.format("hudi").
  options(getQuickstartWriteConfigs).
  // 覆盖写
  option(OPERATION.key(),"insert_overwrite").
  option(PRECOMBINE_FIELD.key(), "ts").
  option(RECORDKEY_FIELD.key(), "uuid").
  option(PARTITIONPATH_FIELD.key(), "partitionpath").
  option(TBL_NAME.key(), tableName).
  mode(Append).
  save(basePath)
```

#### SQL

```sql
-- insert overwrite non-partitioned table
insert overwrite hudi_mor_tbl select 99, 'a99', 20.0, 900;
insert overwrite hudi_cow_nonpcf_tbl select 99, 'a99', 20.0;

-- insert overwrite partitioned table with dynamic partition
insert overwrite table hudi_cow_pt_tbl select 10, 'a10', 1100, '2021-12-09', '10';

-- insert overwrite partitioned table with static partition
insert overwrite hudi_cow_pt_tbl partition(dt = '2021-12-09', hh='12') select 13, 'a13', 1100;
```



### 查询数据

```scala
tripsSnapshotDF.createOrReplaceTempView("hudi_trips_snapshot")

spark.sql("select fare, begin_lon, begin_lat, ts from  hudi_trips_snapshot where fare > 20.0").show()
spark.sql("select _hoodie_commit_time, _hoodie_record_key, _hoodie_partition_path, rider, driver, fare from  hudi_trips_snapshot").show()
```

### 增量查询（特定commit之后）

`_hudi_commit_time`是long型的时间戳，如`20220511151803960` 

```scala
spark.read.
  format("hudi").
  load(basePath).
  createOrReplaceTempView("hudi_trips_snapshot")

val commits = spark.sql("select distinct(_hoodie_commit_time) as commitTime from  hudi_trips_snapshot order by commitTime").map(k => k.getString(0)).take(50)
val beginTime = commits(commits.length - 2) // commit time we are interested in

// incrementally query data after begin commit
val tripsIncrementalDF = spark.read.format("hudi").
  option(QUERY_TYPE_OPT_KEY, QUERY_TYPE_INCREMENTAL_OPT_VAL).
  option(BEGIN_INSTANTTIME_OPT_KEY, beginTime).
  load(basePath)
tripsIncrementalDF.createOrReplaceTempView("hudi_trips_incremental")

spark.sql("select `_hoodie_commit_time`, fare, begin_lon, begin_lat, ts from  hudi_trips_incremental where fare > 20.0").show()
```

### 点查（特定commit之间）

```scala
val beginTime = "000" // Represents earliest possible commit time
val endTime = commits(commits.length - 2) // commit time we are interested in

//incrementally query data
val tripsPointInTimeDF = spark.read.format("hudi").
  option(QUERY_TYPE_OPT_KEY, QUERY_TYPE_INCREMENTAL_OPT_VAL).
  option(BEGIN_INSTANTTIME_OPT_KEY, beginTime).
  option(END_INSTANTTIME_OPT_KEY, endTime).
  load(basePath)
tripsPointInTimeDF.createOrReplaceTempView("hudi_trips_point_in_time")
spark.sql("select `_hoodie_commit_time`, fare, begin_lon, begin_lat, ts from hudi_trips_point_in_time where fare > 20.0").show()
```



### 时间旅行（Time Travel，特定commit之前）

#### DataFrame

```scala
spark.read.
  format("hudi").
  option("as.of.instant", "2021-07-28 14:11:08.200").
  load(basePath)
```

#### SQL

```sql
select * from hudi_cow_pt_tbl timestamp as of '2022-03-07 09:16:28.100' where id = 1;
```



### 更新数据

#### DataFrame

Append 模式

```scala
df.write.format("hudi").
  options(getQuickstartWriteConfigs).
  option(PRECOMBINE_FIELD_OPT_KEY, "ts").
  option(RECORDKEY_FIELD_OPT_KEY, "uuid").
  option(PARTITIONPATH_FIELD_OPT_KEY, "partitionpath").
  option(TABLE_NAME, tableName).
  mode(Append).
  save(basePath)
```

#### SQL

**Update**，需要指定`preCombineField`字段

```sql
UPDATE tableIdentifier SET column = EXPRESSION(,column = EXPRESSION) [ WHERE boolExpression]

-- 示例
update hudi_mor_tbl set price = price * 2, ts = 1111 where id = 1;
update hudi_cow_pt_tbl set name = 'a1_1', ts = 1001 where id = 1;
-- update using non-PK field
update hudi_cow_pt_tbl set ts = 1001 where name = 'a1';
```

**MergeInto**

```sql
MERGE INTO tableIdentifier AS target_alias
USING (sub_query | tableIdentifier) AS source_alias
ON <merge_condition>
[ WHEN MATCHED [ AND <condition> ] THEN <matched_action> ]
[ WHEN MATCHED [ AND <condition> ] THEN <matched_action> ]
[ WHEN NOT MATCHED [ AND <condition> ]  THEN <not_matched_action> ]

<merge_condition> = A equal bool condition 
<matched_action>  =
  DELETE  |
  UPDATE SET *  |
  UPDATE SET column1 = expression1 [, column2 = expression2 ...]
<not_matched_action>  =
  INSERT *  |
  INSERT (column1 [, column2 ...]) VALUES (value1 [, value2 ...])
  
-- 示例
merge into hudi_mor_tbl as target
using merge_source as source
on target.id = source.id
when matched and flag != 'delete' then
 update set id = source.id, name = source.name, ts = source.ts, dt = source.dt, hh = source.hh
when matched and flag = 'delete' then delete
when not matched then
 insert (id, name, ts, dt, hh) values(source.id, source.name, source.ts, source.dt, source.hh)
;
```

### 删除数据

#### DataFrame

```scala
df.write.format("hudi").
  options(getQuickstartWriteConfigs).
  // DELETE
  option(OPERATION_OPT_KEY,"delete").
  option(PRECOMBINE_FIELD_OPT_KEY, "ts").
  option(RECORDKEY_FIELD_OPT_KEY, "uuid").
  option(PARTITIONPATH_FIELD_OPT_KEY, "partitionpath").
  option(TABLE_NAME, tableName).
  mode(Append).
  save(basePath)
```

#### SQL

```sql
DELETE FROM tableIdentifier [ WHERE BOOL_EXPRESSION]

-- 示例
delete from hudi_cow_nonpcf_tbl where uuid = 1;
delete from hudi_mor_tbl where id % 2 = 0;
-- delete using non-PK field
delete from hudi_cow_pt_tbl where name = 'a1';
```



## 示例

数据写入效率，对于cow及mor表，不存在更新操作时，写入速率接近。



### 问题

Hudi 在 Spark 中只有批处理的模式，因此对于流式写数据时，在同一个分区的数据，由于Compaction导致文件被**频繁的全量读取、添加、全量写入**，性能比较慢：（hudi 0.9， 三台节点）

- COW表，在 300 records每秒的处理量；
- COW表，配置clustering，1300 records每秒的处理量；

写入慢原因：

- 微批模式（间隔时间），无法确定写入的数据量大小，且sink端没有缓存，写入的文件会很小。

- 进行小文件合并，降低写的性能；

  

### 代码

```java
import org.apache.log4j.Logger
import org.apache.spark.sql.catalyst.encoders.RowEncoder
import org.apache.spark.sql.{DataFrame, Row, SaveMode}
import org.apache.spark.sql.functions._
import org.apache.spark.sql.types.{LongType, StringType, StructField, StructType}

object SparkHudi {
  val logger = Logger.getLogger(SparkHudi.getClass)

  def main(args: Array[String]): Unit = {

    val spark = SparkSession
      .builder
      .appName("SparkHudi")
      //.master("local[*]")
      .config("spark.serializer", "org.apache.spark.serializer.KryoSerializer")
      .config("spark.default.parallelism", 9)
      .config("spark.sql.shuffle.partitions", 9)
      .enableHiveSupport()
      .getOrCreate()

    // 添加监听器，每一批次处理完成，将该批次的相关信息，如起始offset，抓取记录数量，处理时间打印到控制台
    spark.streams.addListener(new StreamingQueryListener() {
        override def onQueryStarted(queryStarted: QueryStartedEvent): Unit = {
            println("Query started: " + queryStarted.id)
        }
        override def onQueryTerminated(queryTerminated: QueryTerminatedEvent): Unit = {
            println("Query terminated: " + queryTerminated.id)
        }
        override def onQueryProgress(queryProgress: QueryProgressEvent): Unit = {
            println("Query made progress: " + queryProgress.progress)
        }
    })

    // 定义kafka流
    val dataStreamReader = spark
      .readStream
      .format("kafka")
      .option("kafka.bootstrap.servers", "localhost:9092")
      .option("subscribe", "testTopic")
      .option("startingOffsets", "latest")
      .option("maxOffsetsPerTrigger", 100000)
      .option("failOnDataLoss", false)

    // 加载流数据，这里因为只是测试使用，直接读取kafka消息而不做其他处理，是spark结构化流会自动生成每一套消息对应的kafka元数据，
    // 如消息所在主题，分区，消息对应offset等。
    val df = dataStreamReader.load()
      .selectExpr(
        "topic as kafka_topic",
        "CAST(partition AS STRING) kafka_partition",
        "cast(timestamp as String) kafka_timestamp",
        "CAST(offset AS STRING) kafka_offset",
        "CAST(key AS STRING) kafka_key",
        "CAST(value AS STRING) kafka_value",
        "current_timestamp() current_time",
       )
       .selectExpr(
        "kafka_topic",
        "concat(kafka_partition,'-',kafka_offset) kafka_partition_offset",
        "kafka_offset",
        "kafka_timestamp",
        "kafka_key",
        "kafka_value",
        "substr(current_time,1,10) partition_date")

    // 创建并启动query
    val query = df
      .writeStream
      .queryName("demo")
      .foreachBatch { (batchDF: DataFrame, _: Long) => {
        batchDF.persist()

        println(LocalDateTime.now() + "start writing cow table")
        batchDF.write.format("org.apache.hudi")
            .option(TABLE_TYPE_OPT_KEY, "COPY_ON_WRITE")
            .option(PRECOMBINE_FIELD_OPT_KEY, "kafka_timestamp")
            // 以kafka分区和偏移量作为组合主键
            .option(RECORDKEY_FIELD_OPT_KEY, "kafka_partition_offset")
            // 以当前日期作为分区
            .option(PARTITIONPATH_FIELD_OPT_KEY, "partition_date")
            .option(TABLE_NAME, "copy_on_write_table")
            .option(HIVE_STYLE_PARTITIONING_OPT_KEY, true)
            .mode(SaveMode.Append)
            .save("/tmp/sparkHudi/COPY_ON_WRITE")

        println(LocalDateTime.now() + "start writing mor table")
        batchDF.write.format("org.apache.hudi")
            .option(TABLE_TYPE_OPT_KEY, "MERGE_ON_READ")
            .option(PRECOMBINE_FIELD_OPT_KEY, "kafka_timestamp")
            .option(RECORDKEY_FIELD_OPT_KEY, "kafka_partition_offset")
            .option(PARTITIONPATH_FIELD_OPT_KEY, "partition_date")
            .option(TABLE_NAME, "merge_on_read_table")
            .option(HIVE_STYLE_PARTITIONING_OPT_KEY, true)
            .mode(SaveMode.Append)
            .save("/tmp/sparkHudi/MERGE_ON_READ")

        println(LocalDateTime.now() + "finish")
        batchDF.unpersist()
      }
      .option("checkpointLocation", "/tmp/sparkHudi/checkpoint/")
      .start()

    query.awaitTermination()
  }
}
```