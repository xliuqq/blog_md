[toc]

# CarbonData数据定义语言	

## Create Table

```sql
CREATE TABLE [IF NOT EXISTS] [db_name.]table_name[(col_name data_type , ...)]
STORED AS carbondata
[TBLPROPERTIES (property_name=property_value, ...)]
[LOCATION 'path']
```

**TBLPROPERTIES** 的配置如下

### 全局字典配置

- **DICTIONARY_INCLUDE**：默认列不开启词典编码；

- 不支持 Binary 类型和复杂的子列；

### 本地字典配置

  - **LOCAL_DICTIONARY_ENABLE**：默认true，覆盖"carbon.local.dictionary.enable"，默认会对所有的String/Varchar/Char类型列构建词典（除非执行INCLUDE和EXCLUDE）；
  - **LOCAL_DICTIONARY_THRESHOLD**：默认10000，
  - **LOCAL_DICTIONARY_INCLUDE**：默认所有的String/Varchar/Char类型的列；
  - **LOCAL_DICTIONARY_EXCLUDE**：默认none
- 对每个Blocklet中的列进行词典编码；
- 只支持STRING, CHAR, VARCHAR类型；

**系统属性**：

- **carbon.local.dictionary.enable**：默认true；

- **carbon.local.dictionary.decoder.fallback**：默认true，超过阈值会写原先的值；

**注意事项**

- 在局部字典的情况下，由于压缩比较高，生成的块数较少，会导致并发度不够，需要**减少块的大小**。
- **当基数较高但分布在多个负载上时，建议使用本地字典；**
- 在大型集群上，解码数据可能成为全局字典的瓶颈，因为将有许多远程读取。在这种情况下，最好使用本地字典。
- **当基数较小，但加载重复时，最好使用全局字典，因为本地字典在blocklet级别生成多个字典文件以增加冗余。**

### 倒排索引配置

- **INVERTED_INDEX**：默认列不进行倒排索引，倒排索引的列必须指定SORT_COLUMNS；
- **NO_INVERTED_INDEX**：配置不需要的列
- 对于高基数列，可以禁用反向索引以提高数据加载性能；

### 排序列配置

- **SORT_COLUMNS**：默认情况下不对任何列进行排序；
- **SORT_SCOPE**：数据加载期间的指定排序范围
  - NO_SORT：默认范围。它将以未排序的方式加载数据，它将显着提高加载性能。
  - LOCAL_SORT：本地排序（任务级别排序）；
  - BATCH_SORT：如果识别出的块数>并行度，它将提高负载性能，但会降低查询性能。
  - GLOBAL_SORT：提高查询性能，特别是高并发的点查询。（使用spark GroupBy对数据进行排序）

### Table Block大小设置

- **TABLE_BLOCKSIZE**：默认1024 MB，并且支持1 MB到2048 MB的范围；

### Table Blocklet大小设置

- **TABLE_BLOCKLET_SIZE**：在carbondata文件中设置小块大小，默认值为64 MB。**Blocklet是最小的IO读取单位**，点查询时减小blocklet大小可能会提高查询性能。

### Table Page大小设置

- **TABLE_PAGE_SIZE_INMB**：carbondata文件中设置页面大小，默认1MB，支持1 MB到1755 MB(Snappy压缩的最大值）的范围；
- 如果页面大小在32000行之前超过此值，则后续的行写到下一页；
- 配置此属性的较小值（例如1 MB）可以更好地使用CPU缓存页面。

### Table 压缩配置

- 表级别的压缩配置，如果未指定，将使用carbon.properties中的系统级别的配置；
- **MAJOR_COMPACTION_SIZE**：Segments合并成一个前的最大规模；
- **AUTO_LOAD_MERGE**：自动合并Segments；
- **COMPACTION_LEVEL_THRESHOLD**：合并成一个Segments的Segment的数量；
- **COMPACTION_PRESERVE_SEGMENTS**：需要从压缩中排除的最新Segment的数量；
- **ALLOWED_COMPACTION_DAYS**：在配置的时间限制（以天为单位）内生成的段被压缩，而跳过其他；

### 流式配置

- **streaming**：默认false

### 需求列缓存Min/Max

默认情况下，CarbonData缓存Schema中所有列的最小值和最大值。可以配置仅需的列的最大/最小值，不支持binary类型。**在创建表时或创建表后指定要缓存的列**

**COLUMN_META_CACHE**：默认none

- ‘’，表示不需要保存列的最大/最小值；
- ‘col1,col2’，只需要保存col1和col2列的最大最小值；

### Block和Blocklet级缓存

缓存保持在块级别，从而优化了内存的使用，在创建表时或创建表后指定缓存级别。

**CACHE_LEVEL**：BLOCK（默认）或者BLOCKLET

### 支持与Hive/Parquet相同的Flat文件夹

允许所有carbondata和index文件直接保持在tablepath下。方便与其他引擎如Hive、Presto集成。

**flat_folder**：默认为false。

### 字符串超过32000个字符

默认使用Short存储字符串长度，超过的需要用Integer存储，指定列为**LONG_STRING_COLUMNS**（等价于varchar）。

**LONG_STRING_COLUMNS **只能是string / char / varchar列，且不能是全局字典列/排序列/复杂列。

### 表压缩

**carbon.column.compressor**配置为snappy（默认）或者 zstd。

### 不良记录路径

指定将不良记录写入的位置。
```sql
TBLPROPERTIES('BAD_RECORD_PATH'='/opt/badrecords')  
```

### 加载最小数据量

指示每个节点用于数据加载的最小输入数据大小（默认节点均分）。默认情况下未启用。

```sql
TBLPROPERTIES('LOAD_MIN_SIZE_INMB'='256')
```

请注意，启用此功能后，为了实现负载平衡，节点读取数据时会忽略数据本地性。

### 范围列

用于指定一列，以按范围对输入数据进行分区。只能配置一列。不支持二进制数据类型。
```sql
TBLPROPERTIES('RANGE_COLUMN'='col1')
```



## CREATE TABLE AS SELECT

从Hive/Parquet/Carbon表中创建新的Carbon表。

```sql
CREATE TABLE [IF NOT EXISTS] [db_name.]table_name 
STORED AS carbondata 
[TBLPROPERTIES (key1=val1, key2=val2, ...)] 
AS select_statement;
```



## CREATE EXTERNAL TABLE

**创建外部表：删除外部表，不会删除文件；**

```sql
CREATE EXTERNAL TABLE [IF NOT EXISTS] [db_name.]table_name 
STORED AS carbondata LOCATION '$FilesPath'
```

**managed table**

ManagedTable具有FACT和Metadata目录。

**非事务表**

非事务表只有carbondata和carbonindex文件，没有metadata文件，SDK当前支持以这种格式写数据。



## CREATE DATABASE

可以指定存储路径：

```sql
CREATE DATABASE [IF NOT EXISTS] database_name [LOCATION path];
```



## TABLE MANAGEMENT

### 查看表

```sql
SHOW TABLES [IN db_Name]
```

### 重命名表

```sql
ALTER TABLE [db_name.]table_name RENAME TO new_table_name
```

### 增加列

```sql
ALTER TABLE [db_name.]table_name ADD COLUMNS (col_name data_type,...)
TBLPROPERTIES('DICTIONARY_INCLUDE'='col_name,...',
'DEFAULT.VALUE.COLUMN_NAME'='default_value')
```

### 删除列

```sql
ALTER TABLE [db_name.]table_name DROP COLUMNS (col_name, ...)
```

### 改变列属性

```sql
ALTER TABLE [db_name.]table_name CHANGE col_old_name col_new_name column_type
```

### SET和UNSET

```sql
ALTER TABLE tablename SET TBLPROPERTIES('LOCAL_DICTIONARY_ENABLE'='false','LOCAL_DICTIONARY_THRESHOLD'='1000','LOCAL_DICTIONARY_INCLUDE'='column1','LOCAL_DICTIONARY_EXCLUDE'='column2')
```

- SORT_COLUMNS的修改，新的加载将使用新的SORT_COLUMNS。用户可以根据查询调整SORT_COLUMNS，但不会直接影响旧数据。因此，它不会影响未按新SORT_COLUMNS排序的旧数据段的查询性能。
- SORT_COLUMNS修改不支持流表。
- 如果从新的SORT_COLUMNS中删除了反向索引列，则这些列将不会创建反向索引。但是将保留INVERTED_INDEX的旧配置。

### Index合并

将一个segment下的多个index文件（.carbonindex）合并成一个（.carbonindexmerge）。

```sql
ALTER TABLE [db_name.]table_name COMPACT 'SEGMENT_INDEX'
```

**不支持流式表。**

### 删除表

```sql
DROP TABLE [IF EXISTS] [db_name.]table_name
```

### 刷新表

将Carbon表注册到Hive MetaStore。

```sql
REFRESH TABLE $db_NAME.$table_NAME
```

### 表和列的注释

```sql
CREATE TABLE [IF NOT EXISTS] [db_name.]table_name[(col_name data_type [COMMENT col_comment], ...)]
  [COMMENT table_comment]
STORED AS carbondata
[TBLPROPERTIES (property_name=property_value, ...)]
```

## PARTITION

分区类似Hive/Spark中的分区，使用任意列进行分区。

### 创建分区表

```sql
CREATE TABLE [IF NOT EXISTS] [db_name.]table_name 
  [(col_name data_type , ...)]
  [COMMENT table_comment]
  [PARTITIONED BY (col_name data_type , ...)]
  [STORED BY file_format]
  [TBLPROPERTIES (property_name=property_value, ...)]
```

### 分区查看和删除

```sql
SHOW PARTITIONS [db_name.]table_name

ALTER TABLE table_name DROP [IF EXISTS] PARTITION (part_spec, ...) 
```

### 插入和覆盖分区

**插入或覆盖特定的分区的数据**

```
INSERT OVERWRITE TABLE table_name
PARTITION (column = 'partition_name')
select_statement
```

### 创建Hash分区表

当前的Hash，Range，List分区不支持更新和删除数据。

```sql
CREATE TABLE [IF NOT EXISTS] [db_name.]table_name
                  [(col_name data_type , ...)]
PARTITIONED BY (partition_col_name data_type)
STORED AS carbondata
[TBLPROPERTIES ('PARTITION_TYPE'='HASH',
                'NUM_PARTITIONS'='N' ...)]
```

### 创建范围分区表

```
CREATE TABLE [IF NOT EXISTS] [db_name.]table_name
                  [(col_name data_type , ...)]
PARTITIONED BY (partition_col_name data_type)
STORED AS carbondata
[TBLPROPERTIES ('PARTITION_TYPE'='RANGE',
                'RANGE_INFO'='2014-01-01, 2015-01-01, 2016-01-01, ...')]
```

**RANGE_INFO数据必须是升序。**

### 创建列表分区表

```sql
CREATE TABLE [IF NOT EXISTS] [db_name.]table_name
                  [(col_name data_type , ...)]
PARTITIONED BY (partition_col_name data_type)
STORED AS carbondata
[TBLPROPERTIES ('PARTITION_TYPE'='LIST',
                'LIST_INFO'='A, B, (C, D), E, ...')]
```

### 分区操作

```sql
SHOW PARTITIONS [db_name.]table_name
-- add分区
ALTER TABLE [db_name].table_name ADD PARTITION('new_partition')
-- split分区
ALTER TABLE [db_name].table_name SPLIT PARTITION(partition_id) INTO('new_partition1', 'new_partition2'...)
-- drop分区
ALTER TABLE [db_name].table_name DROP PARTITION(partition_id) [WITH DATA]
```

**Hash分区不支持add, split和drop分区。**

**建议**：

- 分区的列不需要加入排序列中；
- 使用分区列进行过滤。



## BUCKETING

分桶功能可用于将表/分区数据分发/组织到多个文件中，以便在同一文件中存在相似的记录。

需要指定分桶的列以及桶的个数。

```sql
CREATE TABLE [IF NOT EXISTS] [db_name.]table_name
                  [(col_name data_type, ...)]
STORED AS carbondata
TBLPROPERTIES('BUCKETNUMBER'='noOfBuckets',
'BUCKETCOLUMNS'='columnname')
```

- 分桶不支持复杂类型；

  

## CACHE

Carbondata使用LRUCache。

```sql
SHOW METACACHE

SHOW METACACHE ON TABLE tableName

DROP METACACHE ON TABLE tableName
```

以上命令不支持子表。

- 仅在执行select查询后才更新缓存信息。
- 如果是alter table，则在触发任何后续的select查询时，已加载的缓存将失效。
- 仅在查询字典列时才将字典加载到缓存中；