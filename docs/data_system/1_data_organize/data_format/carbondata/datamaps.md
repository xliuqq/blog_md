[toc]

# DataMaps

## Overview

```sql
CREATE DATAMAP [IF NOT EXISTS] datamap_name
[ON TABLE main_table]
USING "datamap_provider"
[WITH DEFERRED REBUILD]
DMPROPERTIES ('key'='value', ...)
AS
  SELECT statement
```

| DataMap Provider | Description                         | DMPROPERTIES                                                 | Management       |
| :--------------- | :---------------------------------- | :----------------------------------------------------------- | :--------------- |
| preaggregate     | 单表预聚合表                        | No DMPROPERTY is required                                    | Automatic        |
| timeseries       | 时间维度汇总表                      | event_time, xx_granularity, please refer to [Timeseries DataMap](http://carbondata.apache.org/timeseries-datamap-guide.html) | Automatic        |
| mv               | 多表预聚合表                        | No DMPROPERTY is required                                    | Manual/Automatic |
| lucene           | lucene 文本列索引                   | index_columns to specifying the index columns                | Automatic        |
| bloomfilter      | bloom filter 对高基数列和空间位置列 | index_columns to specifying the index columns                | Automatic        |

### DataMap Management

#### 自动刷新

不使用`WITH DEFERRED REBUILD`语法的情况下在主表上创建数据映射时。每次向主表加载数据，系统都会**立即自动触发对数据映射的加载**。这两种**数据加载（到主表和数据映射）以事务方式执行**。

根据Segment概念，将数据加载到数据映射是增量的，从而避免了昂贵的总重建费用。

如果用户在**主表上执行以下命令，系统将返回失败**。（拒绝操作）

1. 数据管理命令：`UPDATE/DELETE/DELETE SEGMENT`。
2. Schema管理命令：`ALTER TABLE DROP COLUMN`，`ALTER TABLE CHANGE DATATYPE`， `ALTER TABLE RENAME`。请注意，支持添加新列，对于删除列和更改数据类型命令，CarbonData将检查它是否会影响预聚合表，如果没有，则允许该操作，否则将抛出异常而拒绝该操作。
3. 分区管理命令：`ALTER TABLE ADD/DROP PARTITION`。

如果用户确实要在主表上执行上述操作，则**可以先删除数据图，执行该操作**，然后再次重新创建数据图。

如果**用户删除主表，则数据映射也将立即删除**。

#### 手动刷新

用户发出`REBUILD DATAMAP`命令来构建该数据映射之前，查询将不会使用该数据映射。对于每个REBUILD DATAMAP命令，系统将**触发对数据映射的完全重建**。重建完成后，系统会将datamap状态更改为*enabled*，以便可以在查询重写中使用它。

对于每个**新的数据加载、数据更新、删除，将*禁用*相关的数据映射**，这意味着在再次*启用*该数据映射之前，以下查询将无法从该数据映射中受益。

如果用户删除了主表，则相关的数据映射将立即被删除。

**注意事项**：

- 如果要在外部表上创建数据映射，则需要对数据映射进行手动管理。
- 对于诸如BloomFilter数据映射之类的**索引数据映射，无需进行手动刷新**。默认情况下，它是自动刷新的，这意味着在创建数据映射表或加载主表后，其数据将立即刷新。手动刷新此数据图不会产生任何影响

### DataMap目录

当用户创建数据映射时，系统会将数据映射元数据存储在HDFS（可配置）中。

- DataMapSchema文件。它是一个json文件，其中**包含一个数据映射的schema**；

- DataMapStatus文件。只有一个文件，json格式，并且**文件中的每个条目代表一个数据映射**。

DataMapCatalog接口可以获取所有datamap的schema。

### DataMap相关命令

#### Explain

用户可以设置`enable.query.statistics = true`并使用EXPLAIN命令知道。

#### Show DataMap

命令：**SHOW DATAMAPS**，显示一下信息

- DataMapName
- DataMapProviderName，例如mv，preaggreagte，timeseries等
- 关联表
- DataMap属性
- DataMap状态（启用/禁用）
- 同步状态-显示与数据映射表同步的主表的最后一个段ID及其加载结束时间（仅适用于mv数据映射）

### 在DataMap上压缩

仅适用于预聚合数据映射（preaggregate）。

`ALTER TABLE COMPACT`在主表上运行压缩命令**不会自动**压缩在主表上创建的预聚合表。用户需要在**每个预聚合表上分别运行Compaction命令**以对其进行压缩。

## PreAggregate

## Time

## MV

## BloomFilter