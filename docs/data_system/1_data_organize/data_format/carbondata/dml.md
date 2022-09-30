[toc]

# Data Manipulation Language

## 数据加载

CSV格式的数据加载到Carbon 表中。

```sql
LOAD DATA INPATH 'folder_path'
INTO TABLE [db_name.]table_name 
OPTIONS(property_name=property_value, ...)
```

OPTIONS 可配：

- ##### DELIMITER：分隔符

- ##### QUOTECHAR：引用符

- ##### COMMENTCHAR：注释符

- ##### HEADER：默认true

- ##### FILEHEADER：HEADER为false时，提供列名；

- ##### SORT_SCOPE：当前表的SORT_SCOPE；

- ##### MULTILINE

- ##### ESCAPECHAR

- ##### SKIP_EMPTY_LINE

- ##### COMPLEX_DELIMITER_LEVEL_1，COMPLEX_DELIMITER_LEVEL_2，COMPLEX_DELIMITER_LEVEL_3

- ##### ALL_DICTIONARY_PATH：字典文件路径

- ##### COLUMNDICT：指定列的字段文件路径

- ##### DATEFORMAT/TIMESTAMPFORMAT

- ##### SORT COLUMN BOUNDS：仅当LOCAL_SORT

- ##### SINGLE_PASS：默认false，单次通过加载使单个作业可以通过词典的动态生成来完成数据加载。

- ##### BAD RECORDS HANDLING

  - 在处理错误之前，加载所有数据。
  - 在加载数据之前清理或删除不良记录，或者在发现不良记录时停止加载。

- ##### GLOBAL_SORT_PARTITIONS

  - SORT_SCOPE定义为GLOBAL_SORT时指定的进行排序时要使用的分区数。

- ##### SCALE_FACTOR

  - 对于RANGE_COLUMN，SCALE_FACTOR用于控制范围分区的数量，计算公式如下：

    ```java
    splitSize = max(blocklet_size, (block_size - blocklet_size)) * scale_factor
    numPartitions = total size of input data / splitSize
    ```

  **注意：**

  - 如果同时使用GLOBAL_SORT_PARTITIONS和SCALE_FACTOR，则仅GLOBAL_SORT_PARTITIONS有效。
  - 默认情况下，对RANGE_COLUMN的压缩将使用LOCAL_SORT。

- ##### CARBON_ENABLE_RANGE_COMPACTION

  - 配置是否将基于范围的压缩用于RANGE_COLUMN。默认值是true。

## 数据插入

源表可以是Hive表，Parquet表或者Carbon表

```sql
INSERT INTO|OVERWRITE TABLE <CARBONDATA TABLE> SELECT * FROM sourceTableName 
[ WHERE { <filter_condition> } ]
```

- 源表和carbon表的schema必须一致；
- 源和目标表的列的数据类型需要一致；
- INSERT INTO如果发现bad record，则会全部失败；
- 从源表到目标表的插入过程中，无法在源表中加载或更新数据。

## 加载数据到静态分区

将数据加载到指定的分区中

```sql
LOAD DATA INPATH 'folder_path'
INTO TABLE [db_name.]table_name PARTITION (partition_spec) 
OPTIONS(property_name=property_value, ...)

INSERT INTO TABLE [db_name.]table_name PARTITION (partition_spec) <SELECT STATEMENT>
```

## 加载数据到动态分区

此命令允许您使用动态分区加载数据。如果未指定分区规范，则该分区被视为动态分区。

```sql
LOAD DATA INPATH '${env:HOME}/staticinput.csv'
INTO TABLE locationTable

INSERT INTO TABLE locationTable
SELECT <columns list excluding partition columns> FROM another_user
```

## 更新和删除

基于列表达式和可选过滤条件来更新CarbonData表

```sql
UPDATE <table_name> 
SET (column_name1, column_name2, ... column_name n) = (column1_expression , column2_expression, ... column n_expression )
[ WHERE { <filter_condition> } ]

UPDATE <table_name>
SET (column_name1, column_name2) =(select sourceColumn1, sourceColumn2 from sourceTable [ WHERE { <filter_condition> } ] )
[ WHERE { <filter_condition> } ]

DELETE FROM table_name [WHERE expression]
```

## 压缩

```sql
ALTER TABLE [db_name.]table_name COMPACT 'MINOR/MAJOR/CUSTOM'
```

**minor**

在次要压缩中，用户可以指定要合并的载荷数。如果参数carbon.enable.auto.load.merge设置为true，则**每次数据加载都会触发次要压缩**。如果有任何段可以合并，则压缩将与数据加载并行进行，次要压缩分为两个级别：

- 级别1：合并尚未压缩的Segment。
- 级别2：再次合并压缩的Segment以形成更大的Segment。

**major**

在重大压缩中，可以将多个段合并为一个大段。用户将指定压缩大小，直到可以合并各个段为止。通常在非高峰时间进行大型压缩。为属性carbon.major.compaction.size配置适当的MB值。

**custom**

在“自定义压缩”中，用户可以直接**指定要合并为一个大段的段ID**。所有指定的段ID应该存在并且有效，否则压缩将失败。自定义压缩通常在非高峰时间完成。

```sql
ALTER TABLE table_name COMPACT 'CUSTOM' WHERE SEGMENT.ID IN (2,3,4)
```

**压缩后清除**

清除压缩的段

```sql
CLEAN FILES FOR TABLE carbon_table
```

## Segment管理

### 查看Segment

```sql
SHOW [HISTORY] SEGMENTS FOR TABLE [db_name.]table_name LIMIT number_of_segments
```

### 根据ID删除Segemnt

```sql
DELETE FROM TABLE [db_name.]table_name WHERE SEGMENT.ID IN (segment_id1, ...)
```

### 根据Date删除Segemnt

```sql
DELETE FROM TABLE [db_name.]table_name WHERE SEGMENT.STARTTIME BEFORE DATE_VALUE
```

### 查询特定段的数据

```sql
SET carbon.input.segments.<database_name>.<table_name> = <list of segment IDs>

-- 多线程
CarbonSession.threadSet ("carbon.input.segments.<database_name>.<table_name>","<list of segment IDs>");

-- reset
SET carbon.input.segments.<database_name>.<table_name> = *;

```