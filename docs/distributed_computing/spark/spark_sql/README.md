# Spark SQL

http://spark.apache.org/docs/latest/sql-programming-guide.html



## DataFrame

Koalas：基于 Spark DataFrame 实现的分布式 Pandas DataFrame，已经被集成到 Spark 3.x 中。



## SQL语法

### 辅助语句

#### refresh

`REFRESH` is used to invalidate and refresh all the cached data (and the associated metadata) for all Datasets that contains the given data source path. Path matching is by prefix, i.e. “/” would invalidate everything that is cached.

```sql
df = spark.read.format("hudi").load(basePath+"/*/*")
df.createOrReplaceTempView("track_log")
# 刷新表的元数据和data
spark.sql("refresh table mytable")
```



## Catalog

Catalog 主要**用于各种函数资源信息和元数据信息（数据库、数据表、数据视图、数据分区与函数等)的统一管理**。

### SessionCatalog

> 在 Spark 2.x 中，Spark SQL中的 Catalog 体系实现以 `SessionCatalog` 为主体，通过`SparkSession` （Spark程序入口）提供给外部调用。

一般一个`SparkSession`对应一个`SessionCatalog`。

**本质上， `SessionCatalog`起到了一个代理的作用，对底层的元数据信息、临时表信息、视图信息和函数信息进行了封装**。

- **GlobalTempViewManager**(全局的临时视图管理)：用于 Session 间（同进程）共享全局临时表；
- **FunctionResourceLoader**(函数资源加载器) ：除内置实现的各种函数外，还支持用户自定义的函数和Hive中的各种函数
- **FunctionRegistry**(函数注册接口)：实现对**函数的注册(Register)、查找(Lookup)和删除(Drop)**等功能
- **ExternalCatalog**：用来管理**数据库(Databases)、数据表(Tables)、数据分区(Partitions)和函数(Functions)**的接口，具体实现有
  - `InMemoryCatalog`：将上述信息存储在内存中，一般用于测试或比较简单的SQL处理（没有metastore信息），重启无法识别之前创建过的数据。
  - `HiveExternalCatalog`：实现持久化的管理，应用在生产环境中

### V2SessionCatalog

**TableCatalog**：**定义 catalog API 用来 create，alter，load 和 drop 表**

**FunctionCatalog**：**定义 catalog API 用来操作函数**

**SupportsNamespaces**：**定义 catalog API 用来操作 Namespace**



**V2SessionCatalog**：保证后向兼容性，是对 SessionCatalog 的封装。



### 自定义Catalog

TODO



### CatalogManager

Spark 通过CatalogManager 可以同时管理内部连接多个catalog，通过 `spark.sql.catalog.${name}` 可以注册多个catalog，Spark默认的catalog由 `spark.sql.catalog.spark_catalog` 参数指定。通常的做法是，自定义catalog类继承 `DelegatingCatalogExtension 实现`，然后通过 `spark.sql.catalog.spark_catalog` 参数来指定自定义catalog类。

- 如 Hudi 使用时的 `org.apache.spark.sql.hudi.catalog.HoodieCatalog`

多个 catalog（比如两个 hive metatore） 时可以使用全路径访问表名

```sql
select * from spark_catalog.database_1.table_1
```



## [Hive](https://spark.apache.org/docs/latest/sql-data-sources-hive-tables.html)

> Spark 可以读写 Hive 中的 表。

```scala
val spark = SparkSession
  .builder()
  .appName("Spark Hive Example")
  .config("spark.sql.warehouse.dir", warehouseLocation)
  // 必须有 hive 的 classpath
  .enableHiveSupport()
  .getOrCreate()

// saveAsTable 将表写入到 hive 中
df.write.mode(SaveMode.Overwrite).saveAsTable("hive_records")

```

- 如果没配置 hive-site.xml，则数据会存储在`spark.sql.warehouse.dir`（默认application启动的当前目录的`spark-warehouse`）下，同时当前目录下创建`metastore_db`存储元数据（通过 Derby）。

