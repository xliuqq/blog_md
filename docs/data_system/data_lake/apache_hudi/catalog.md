# Catalog

## Spark Hudi Catalog

Spark 使用 Hudi 时，需要配置

- `spark.sql.catalog.spark_catalog=org.apache.spark.sql.hudi.catalog.HoodieCatalog`



## Flink using Hudi Catalog

### [Create Catalog](https://hudi.apache.org/docs/sql_ddl#create-catalog)

用于管理 SQL 表，在多个 Session中共享。

- For `hms` mode, the catalog also supplements the hive syncing options.

**Example**

```sql
CREATE CATALOG hoodie_catalog
  WITH (
    'type'='hudi',
    'catalog.path' = '${catalog default root path}',
    'hive.conf.dir' = '${directory where hive-site.xml is located}',
    'mode'='hms' -- supports 'dfs' mode that uses the DFS backend for table DDLs persistence
  );

--- 创建数据库供hudi使用
create database hudi.hudidb;

--- order表
CREATE TABLE hudi.hudidb.orders_hudi(
  uuid INT,
  ts INT,
  num INT,
  PRIMARY KEY(uuid) NOT ENFORCED
) WITH (
  'connector' = 'hudi',
  'table.type' = 'MERGE_ON_READ'
);

select * from hudi.hudidb.orders_hudi;
```



## Sync to Catalogs

### Hive MetaStore

Hudi 支持将表信息同步给 Catalog，如 [Hive MetaStore](https://hudi.apache.org/docs/syncing_metastore)。