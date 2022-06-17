# Table API Connector

## Flink 1.13 支持的Connector

| Name                                                         |                           Version                            |                Source                |            Sink            |
| :----------------------------------------------------------- | :----------------------------------------------------------: | :----------------------------------: | :------------------------: |
| [Filesystem](https://ci.apache.org/projects/flink/flink-docs-release-1.13/docs/connectors/table/filesystem/) |                                                              |  Bounded and Unbounded Scan, Lookup  | Streaming Sink, Batch Sink |
| [Elasticsearch](https://ci.apache.org/projects/flink/flink-docs-release-1.13/docs/connectors/table/elasticsearch/) |                          6.x & 7.x                           |            Not supported             | Streaming Sink, Batch Sink |
| [Apache Kafka](https://ci.apache.org/projects/flink/flink-docs-release-1.13/docs/connectors/table/kafka/) |                            0.10+                             |            Unbounded Scan            | Streaming Sink, Batch Sink |
| [Amazon Kinesis Data Streams](https://ci.apache.org/projects/flink/flink-docs-release-1.13/docs/connectors/table/kinesis/) |                                                              |            Unbounded Scan            |       Streaming Sink       |
| [JDBC](https://ci.apache.org/projects/flink/flink-docs-release-1.13/docs/connectors/table/jdbc/) |                                                              |         Bounded Scan, Lookup         | Streaming Sink, Batch Sink |
| [Apache HBase](https://ci.apache.org/projects/flink/flink-docs-release-1.13/docs/connectors/table/hbase/) |                        1.4.x & 2.2.x                         |         Bounded Scan, Lookup         | Streaming Sink, Batch Sink |
| [Apache Hive](https://ci.apache.org/projects/flink/flink-docs-release-1.13/docs/connectors/table/hive/overview/) | [Supported Versions](https://ci.apache.org/projects/flink/flink-docs-release-1.13/docs/connectors/table/hive/overview/#supported-hive-versions) | Unbounded Scan, Bounded Scan, Lookup | Streaming Sink, Batch Sink |

### Kafka



### JDBC

#### maven

```xml
<dependency>
  <groupId>org.apache.flink</groupId>
  <artifactId>flink-connector-jdbc_2.11</artifactId>
  <version>1.13.0</version>
</dependency>
```

| Driver     | Group Id           | Artifact Id            | JAR                                                          |
| :--------- | :----------------- | :--------------------- | :----------------------------------------------------------- |
| MySQL      | `mysql`            | `mysql-connector-java` | [Download](https://repo.maven.apache.org/maven2/mysql/mysql-connector-java/) |
| PostgreSQL | `org.postgresql`   | `postgresql`           | [Download](https://jdbc.postgresql.org/download.html)        |
| Derby      | `org.apache.derby` | `derby`                | [Download](http://db.apache.org/derby/derby_downloads.html)  |

#### 创建表

```sql
- register a MySQL table 'users' in Flink SQL
CREATE TABLE MyUserTable (
  id BIGINT,
  name STRING,
  age INT,
  status BOOLEAN,
  PRIMARY KEY (id) NOT ENFORCED
) WITH (
   'connector' = 'jdbc',
   'url' = 'jdbc:mysql://localhost:3306/mydatabase',
   'table-name' = 'users',
   'driver' = 'com.mysql.cj.jdbc.Driver',
   'username' = 'root',
   'password' = 'root'
);

-- write data into the JDBC table from the other table "T"
INSERT INTO MyUserTable
SELECT id, name, age, status FROM T;

-- scan data from the JDBC table
SELECT id, name, age, status FROM MyUserTable;

-- temporal join the JDBC table as a dimension table
SELECT * FROM myTopic
LEFT JOIN MyUserTable FOR SYSTEM_TIME AS OF myTopic.proctime
ON myTopic.key = MyUserTable.id;
```

#### Sink

| Option                     | Required | Default | Type     | Description                                                  |
| :------------------------- | :------- | :------ | :------- | :----------------------------------------------------------- |
| sink.buffer-flush.max-rows | optional | 100     | Integer  | flush前的最大缓冲数据量，0表示禁用。                         |
| sink.buffer-flush.interval | optional | 1s      | Duration | flush间隔，异步线程刷写数据，0表示禁用。 `'sink.buffer-flush.max-rows'`设置为0，可以通过此刷写时间做异步数据刷写。 |
| sink.max-retries           | optional | 3       | Integer  | 写数据库失败的最大尝试次数。                                 |
| sink.parallelism           | optional | (none)  | Integer  | Defines the parallelism of the JDBC sink operator. By default, the parallelism is determined by the framework using the same **parallelism of the upstream chained operator.** |

使用upsert语义进行插入，通过主键提供幂等性。

如果出现失败，Flink作业将从最后一个成功的检查点恢复并重新处理，这可能导致在恢复期间重新处理消息。强烈推荐使用upsert模式，因为如果需要重新处理记录，它有助于避免违反约束或重复数据。



#### Lookup Cache

JDBC 连接可以作为临时的join表数据源（例如维度表），当前只支持**同步查找**模式。

| Option                | Required | Default | Type     | Description                                                  |
| :-------------------- | :------- | :------ | :------- | :----------------------------------------------------------- |
| lookup.cache.max-rows | optional | (none)  | Integer  | 查找cache的最大行数，超过后，最旧的一行会过期。默认禁用      |
| lookup.cache.ttl      | optional | (none)  | Duration | cache中每行的ttl存活时间，超时后，最旧的数据过期。默认禁用。 |
| lookup.max-retries    | optional | 3       | Integer  | 数据库查找失败的重试次数                                     |

#### Postgres Database as a Catalog（TODO）



#### 数据类型映射

| MySQL type                            | PostgreSQL type                                              | Flink SQL type                       |
| :------------------------------------ | :----------------------------------------------------------- | :----------------------------------- |
| `TINYINT`                             |                                                              | `TINYINT`                            |
| `SMALLINT` `TINYINT UNSIGNED`         | `SMALLINT` `INT2` `SMALLSERIAL` `SERIAL2`                    | `SMALLINT`                           |
| `INT` `MEDIUMINT` `SMALLINT UNSIGNED` | `INTEGER` `SERIAL`                                           | `INT`                                |
| `BIGINT` `INT UNSIGNED`               | `BIGINT` `BIGSERIAL`                                         | `BIGINT`                             |
| `BIGINT UNSIGNED`                     |                                                              | `DECIMAL(20, 0)`                     |
| `BIGINT`                              | `BIGINT`                                                     | `BIGINT`                             |
| `FLOAT`                               | `REAL` `FLOAT4`                                              | `FLOAT`                              |
| `DOUBLE` `DOUBLE PRECISION`           | `FLOAT8` `DOUBLE PRECISION`                                  | `DOUBLE`                             |
| `NUMERIC(p, s)` `DECIMAL(p, s)`       | `NUMERIC(p, s)` `DECIMAL(p, s)`                              | `DECIMAL(p, s)`                      |
| `BOOLEAN` `TINYINT(1)`                | `BOOLEAN`                                                    | `BOOLEAN`                            |
| `DATE`                                | `DATE`                                                       | `DATE`                               |
| `TIME [(p)]`                          | `TIME [(p)] [WITHOUT TIMEZONE]`                              | `TIME [(p)] [WITHOUT TIMEZONE]`      |
| `DATETIME [(p)]`                      | `TIMESTAMP [(p)] [WITHOUT TIMEZONE]`                         | `TIMESTAMP [(p)] [WITHOUT TIMEZONE]` |
| `CHAR(n)` `VARCHAR(n)` `TEXT`         | `CHAR(n)` `CHARACTER(n)` `VARCHAR(n)` `CHARACTER VARYING(n)` `TEXT` | `STRING`                             |
| `BINARY` `VARBINARY` `BLOB`           | `BYTEA`                                                      | `BYTES`                              |
|                                       | `ARRAY`                                                      | `ARRAY`                              |

### DataGen（数据生成）

### Print（控制台打印）