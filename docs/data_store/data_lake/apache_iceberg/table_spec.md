[toc]

# 表规范定义（v0.11）

## 版本说明

### Version 1：Analytic Data Tables

当前版本，定义如何通过不可变的文件格式（如Avro、Parquet、ORC）管理大规模分析表。

### Version 2：Row-level Deletes

版本2的目标是提供**一种对行级删除进行编码**的方法。此更新可用于删除或替换不可变数据文件中的个别行，而无需重写该文件。

社区正在进行中，还未完成。

## 目标

### 可序列化的隔离

读取将与并发写隔离，并且始终使用表数据的已提交快照。写入将支持在单个操作中删除和添加文件，并且永远不会部分可见。读取将不会获得锁。

### 速度

使用O(1)远程调用来计划扫描的文件，而不是O(n)，其中n随着表的大小增长，如分区或文件的数量。

### 规模

**作业计划将主要由客户端处理**，而不是在中央元数据存储上出现瓶颈。**元数据将包括基于成本的优化所需的信息**。

### 演进

表将支持完整的模式和分区规范的演进。模式演化支持安全的列添加、删除、重新排序和重命名，包括在嵌套结构中。

### 可靠类型

table将为一组核心类型提供良好定义和可靠的支持。

### 存储分离

分区将是表配置。读取计划将使用数据值上的谓词，而不是分区值。表将支持演进的分区方案。

### 格式

底层数据文件格式将支持相同的模式演化规则和类型。读和写优化的格式都可用。

## 概览

表格式跟踪表中的单个数据文件，而不是目录。

表状态保存在元数据文件中。对表状态的所有更改都会创建一个新的元数据文件，并用原子交换替换旧的元数据。表元数据文件跟踪表结构、分区配置、自定义属性和表内容的快照。快照表示某个时间表的状态，用于访问表中的完整数据文件集。

快照中的数据文件由一个或多个清单文件跟踪，清单文件中每行表示表中的一个数据文件、文件的分区数据及其指标。快照中的数据是其清单中所有文件的并集。**清单文件可在快照之间重用，以避免重写变化缓慢的元数据**。清单可以跟踪具有表的任何子集的数据文件，并且不与分区关联。

构成快照的清单清单存储在清单列表文件中。每个清单列表存储有关清单的元数据，包括分区统计信息和数据文件计数。这些统计信息用于避免读取操作不需要的清单。

![Iceberg snapshot structure](pics/iceberg-metadata.png)

### 乐观并行

表元数据文件的原子交换提供了可序列化隔离的基础。当读取表元数据时，读者使用当前的快照，并不会受到更改的影响，直到它们刷新并获取新的元数据位置。

写入者乐观地创建表元数据文件，假设在提交者提交之前不会更改当前版本。写入者创建更新后，它将通过将表的元数据文件指针从基本版本交换为新版本来进行提交。

如果更新所基于的快照不再是当前快照，则写入者必须根据新的当前版本重试更新。在明确定义的条件下，某些操作通过重新应用元数据更改并提交来支持重试。例如，如果所有重写的文件仍在表中，则可以将重写文件的更改应用于新的表快照。

写操作成功提交所需的条件决定了隔离级别。写入者可以选择要验证的内容，并可以做出不同的隔离保证。

### 序列号

数据和删除文件的相对年龄依赖于分配给每一个成功提交的序列号。当为commit创建快照时,它是优化地分配下一个序列号，并将其写入快照的元数据。如果提交失败并必须重新尝试，序列号将重新分配并写入新的快照元数据。

为快照创建的文件（所有的清单、数据文件和删除文件）都继承了快照的序列号。**清单列表上的清单文件的元数据存储一个清单的序列号**。新数据和元数据文件条目以一个序列号为null，在读取时被替换为manifest的序列号。当数据或删除文件写入一个新的清单(作为“现有的”)时，继承的序列号被写入以确保它在第一次继承后不会改变。

继承清单元数据的序列号允许编写一个新的清单一次并重新使用它。为了重新尝试，要更改一个序列号，只有**清单列表必须重写**（该文件总是会被根据最新的清单文件集合重写）。

### 行级删除

行级删除存储在删除文件中。有两种编码方式：

- 位置删除（Position deletes）：指定数据文件的路径以及行的位置，标记一行删除；
- 相等删除（Equality deletes）：执行一个或多个列的值标记一行删除，比如id=5的行删除。

与数据文件一样，通过分区跟踪删除文件。一般来说，删除文件必须应用于与相同分区的旧数据文件（参见扫描计划（Scan Planning）详细信息）。可以使用列的指标度量数据来确定删除文件的行是否会和数据文件/扫描范围的内容相重叠。

### 文件系统操作

Iceberg依赖文件系统支持以下操作：

- In-place write：文件在写入时不会被移动和改动；
- Seekable reads：数据文件的格式支持随机读；
- Deletes：表的删除文件不会再被使用。

这些需求与对象存储器兼容，比如S3。

表不需要随机访问写入操作。一旦写入，数据和元数据文件是不可变的，直到删除它们。

表不需要重命名操作，除了使用原子重命名来实现新的元数据文件的提交操作的表之外。

## 规范

http://iceberg.apache.org/spec.

### 模式和数据类型

嵌套类型：

- struct：tuple of typed values；
- list：同一个类型的数组；
- map：k-v对，key字段和value字段都有唯一的id。

基本类型(：

- boolean，int，long，float，double，decimal(P,S)；
- date，time，timestamp，timestamptz；
- string，uuid，fixed(L)，binary。

#### 模式演化

任何结构，包括顶级模式，支持的模式演化：

- 基本类型提升（int->long，float->double，decimal(P,S)->decimal(P', S) if P' > P）
- 添加新的字段；
- 重命名已有的字段，修改名称，但是不修改id；
- 重排序已有的字段；
- 删除字段；

其它的模式改变不支持演化，如将`map<string, int> ↔ map<string, struct<int>>`。

#### 列投影

**根据字段id选取列**，因为表模式中的列名和顺序可以在数据文件写入后改变。如果数据文件中没有该字段id，则每行该字段对应的值为null。

#### 保留的字段IDs

字段id不能超过2147483447（Integer.MAX-200），保留的ID用于元数据列，使用在user data schemas，比如`_file`列保存行存储的文件路径。

metadata columns is:

| Field id, name             | Type          | Description                                                  |
| :------------------------- | :------------ | :----------------------------------------------------------- |
| **`2147483646 _file`**     | `string`      | Path of the file in which a row is stored                    |
| **`2147483645 _pos`**      | `long`        | Ordinal position of a row in the source data file            |
| **`2147483546 file_path`** | `string`      | Path of a file, used in position-based delete files          |
| **`2147483545 pos`**       | `long`        | Ordinal position of a row, used in position-based delete files |
| **`2147483544 row`**       | `struct<...>` | Deleted row values, used in position-based delete files      |

### 分区

对于存储在单个数据文件中的所有记录，数据文件的分区值必须相同。

**分区规范（parition spec）**的组成：

- 源列id（source column id）：来源于表模式，必须是基本类型或者嵌套于struct（不能是map或者list）；
- 分区字段id（partition field id）：标识一个分区字段，并且在单个分区规范中是唯一的。在**v2表元数据中，它在所有分区规范中都是唯一的**。
- 转换（transform）：根据源列产生分区值的函数；
- 分区名（partition name）；

#### 转换算子

针对`null`输入值，必须返回`null`输出。

| Transform         | Description                                                  | Source types                                                 | Result type |
| :---------------- | :----------------------------------------------------------- | :----------------------------------------------------------- | :---------- |
| **`identity`**    | Source value, unmodified                                     | Any                                                          | Source type |
| **`bucket[N]`**   | Hash of value, mod `N` (32-bit Murmur3 hash)                 | `int`, `long`, `decimal`, `date`, `time`, `timestamp`, `timestamptz`, `string`, `uuid`, `fixed`, `binary` | `int`       |
| **`truncate[W]`** | Value truncated to width `W` (see below)                     | `int`, `long`, `decimal`, `string`                           | Source type |
| **`year`**        | Extract a date or timestamp year, as years from 1970         | `date`, `timestamp(tz)`                                      | `int`       |
| **`month`**       | Extract a date or timestamp month, as months from 1970-01-01 | `date`, `timestamp(tz)`                                      | `int`       |
| **`day`**         | Extract a date or timestamp day, as days from 1970-01-01     | `date`, `timestamp(tz)`                                      | `date`      |
| **`hour`**        | Extract a timestamp hour, as hours from 1970-01-01 00:00:00  | `timestamp(tz)`                                              | `int`       |

bucket支持在模式演化中对`N`进行修改。

### 排序

`sort order`由sort order id和多个sort字段组成，每个sort field由以下组成：

- source column id；
- transfrom：算子同分区的转换算子。
- sort direction：升序（asc）或降序（desc）；
- null order：null 优先（nulls-first）或null最后（nulls-last）。

order id `0`保留作为不排序的order。

Float排序规则：`-NaN` < `-Infinity` < `-value` < `-0` < `0` < `value` < `Infinity` < `NaN`

在`manifest`文件中定义数据或删除文件中数据的`sort order id`。

可以使用默认排序顺序id配置表，指示新数据在默认情况下应该如何排序。写入器应该使用这个默认排序顺序来对写入的数据进行排序，但如果默认排序的代价太高，就不需要这样做，例如对流写来说排序的代价太高。

### 清单（Mainfest）

清单是一个不可变的Avro文件，它列出数据文件或删除文件，以及每个文件的分区数据元组、指标和跟踪信息。

清单是有效的Iceberg数据文件:文件必须使用有效的Iceberg格式、模式和列投影。

清单可以存储数据文件或删除文件，但不能同时存储数据文件或删除文件，因为在作业计划期间首先扫描包含删除文件的清单。清单是数据清单还是删除清单都存储在清单元数据中。

**清单存储单个的分区规格的文件**。当一个表的分区规格发生变化时，旧文件保留在旧的清单中，新文件被写入新的清单。

清单文件必须将分区规格和其他元数据作为属性存储在Avro文件的键值元数据中：

| v1         | v2         | Key                 | Value                                                        |
| :--------- | :--------- | :------------------ | :----------------------------------------------------------- |
| *required* | *required* | `schema`            | JSON representation of the table schema at the time the manifest was written |
| *required* | *required* | `partition-spec`    | JSON fields representation of the partition spec used to write the manifest |
| *optional* | *required* | `partition-spec-id` | Id of the partition spec used to write the manifest as a string |
| *optional* | *required* | `format-version`    | Table format version number of the manifest as a string      |
|            | *required* | `content`           | Type of content files tracked by the manifest: “data” or “deletes” |

清单文件的模式是一个名为`manifest_entry`的结构体，带有以下字段:

| v1         | v2         | Field id, name          | Type                                                      | Description                                                  |
| :--------- | :--------- | :---------------------- | :-------------------------------------------------------- | :----------------------------------------------------------- |
| *required* | *required* | **`0 status`**          | `int` with meaning: `0: EXISTING` `1: ADDED` `2: DELETED` | Used to track additions and deletions                        |
| *required* | *optional* | **`1 snapshot_id`**     | `long`                                                    | Snapshot id where the file was added, or deleted if status is 2. Inherited when null. |
|            | *optional* | **`3 sequence_number`** | `long`                                                    | Sequence number when the file was added. Inherited when null. |
| *required* | *required* | **`2 data_file`**       | `data_file` `struct` (see below)                          | File path, partition tuple, metrics, …                       |

`data_file` 是个结构体，具备以下字段:

| v1         | v2         | Field id, name                    | Type                                                         | Description                                                  |
| :--------- | :--------- | :-------------------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
|            | *required* | **`134 content`**                 | `int` with meaning: `0: DATA`, `1: POSITION DELETES`, `2: EQUALITY DELETES` | Type of content stored by the data file: data, equality deletes, or position deletes (all v1 files are data files) |
| *required* | *required* | **`100 file_path`**               | `string`                                                     | Full URI for the file with FS scheme                         |
| *required* | *required* | **`101 file_format`**             | `string`                                                     | String file format name, avro, orc or parquet                |
| *required* | *required* | **`102 partition`**               | `struct<...>`                                                | Partition data tuple, schema based on the partition spec     |
| *required* | *required* | **`103 record_count`**            | `long`                                                       | Number of records in this file                               |
| *required* | *required* | **`104 file_size_in_bytes`**      | `long`                                                       | Total file size in bytes                                     |
| *required* |            | ~~**`105 block_size_in_bytes`**~~ | `long`                                                       | **Deprecated. Always write a default in v1. Do not write in v2.** |
| *optional* |            | ~~**`106 file_ordinal`**~~        | `int`                                                        | **Deprecated. Do not write.**                                |
| *optional* |            | ~~**`107 sort_columns`**~~        | `list<112: int>`                                             | **Deprecated. Do not write.**                                |
| *optional* | *optional* | **`108 column_sizes`**            | `map<117: int, 118: long>`                                   | Map from column id to the total size on disk of all regions that store the column. Does not include bytes necessary to read other columns, like footers. Leave null for row-oriented formats (Avro) |
| *optional* | *optional* | **`109 value_counts`**            | `map<119: int, 120: long>`                                   | Map from column id to number of values in the column (including null and NaN values) |
| *optional* | *optional* | **`110 null_value_counts`**       | `map<121: int, 122: long>`                                   | Map from column id to number of null values in the column    |
| *optional* | *optional* | **`137 nan_value_counts`**        | `map<138: int, 139: long>`                                   | Map from column id to number of NaN values in the column     |
| *optional* |            | ~~**`111 distinct_counts`**~~     | `map<123: int, 124: long>`                                   | **Deprecated. Do not write.**                                |
| *optional* | *optional* | **`125 lower_bounds`**            | `map<126: int, 127: binary>`                                 | Map from column id to lower bound in the column serialized as binary [1]. Each value must be less than or equal to all non-null, non-NaN values in the column for the file [2] |
| *optional* | *optional* | **`128 upper_bounds`**            | `map<129: int, 130: binary>`                                 | Map from column id to upper bound in the column serialized as binary [1]. Each value must be greater than or equal to all non-null, non-Nan values in the column for the file [2] |
| *optional* | *optional* | **`131 key_metadata`**            | `binary`                                                     | Implementation-specific key metadata for encryption          |
| *optional* | *optional* | **`132 split_offsets`**           | `list<133: long>`                                            | Split offsets for the data file. For example, all row group offsets in a Parquet file. Must be sorted ascending |
|            | *optional* | **`135 equality_ids`**            | `list<136: int>`                                             | Field ids used to determine row equality in equality delete files. Required when `content=2` and should be null otherwise. Fields with ids listed in this column must be present in the delete file |
| *optional* | *optional* | **`140 sort_order_id`**           | `int`                                                        | ID representing sort order for this file [3].                |

注意：

- 如果排序顺序ID缺失或未知，则假定该顺序是未排序的。只有数据文件和相等删除文件应该使用非空的顺序id进行写入。位置删除文件需要按文件和位置排序，而不是表顺序，并且应该将排序顺序id设置为null，读者必须忽略位置删除文件的排序顺序id。

分区结构存储每个文件分区值的元组。它的类型派生于用于编写清单文件的分区规范的分区字段。在v2中，分区结构的字段id必须与分区规范中的id匹配。

#### Manifest Entry Fields

**清单输入字段（Manifest Entry Fields）用于跟踪快照中文件的添加或逻辑上的删除**。`data_file`结构被嵌套在清单条目中，因此可以在不使用清单条目字段的情况下轻松地将其传递给作业计划。

当一个文件添加到数据集时，它的清单条目应该存储添加文件的快照ID，并将status设置为1(已添加)。

当从数据集中替换或删除文件时，它的清单条目字段存储删除文件的快照ID和状态2(已删除)。当被删除的快照被垃圾回收时，文件可能会从文件系统中被删除，假设旧的快照也被垃圾回收了[1]。

Iceberg v2向条目新增序列号，并使快照id成为可选的。s**equence_number和snapshot_id这两个字段在为空时都是从清单元数据继承**。也就是说，如果某个条目的字段为空，那么该条目必须从清单文件的元数据中继承其值，该元数据存储在清单列表[2]中。

注意：

1. 从技术上讲，当包含文件作为“活动”数据的最后一个快照被垃圾收集时，可以删除数据文件。但这更难检测，因为需要找到多个快照的差异。**跟踪快照中删除了哪些文件，并在快照过期时删除这些文件**，这更加容易。
2. v2中需要清单列表文件，因此要继承的sequence_number和snapshot_id总是可用的。

#### 序列号继承

当数据或删除文件添加到表中时，清单跟踪序列号。

在添加新文件时，它的序号被设置为null，因为在成功提交快照之前，不会分配快照的序号。在读取时，通过用清单列表中的清单序号替换null继承序列号。

当将现有文件写入新的清单时，序号必须为非空，并设置为继承的序号。

通过元数据树继承序列号允许在没有已知序列号的情况下写入新的清单，因此清单可以写入一次，并在提交重试时重用。要更改序列号进行重试，只有清单列表必须被重写。

当读取v1时显示没有序列号列，所有文件的序列号必须默认为0。

### 快照

由以下字段组成：

| v1         | v2         | Field                    | Description                                                  |
| :--------- | :--------- | :----------------------- | :----------------------------------------------------------- |
| *required* | *required* | **`snapshot-id`**        | A unique long ID                                             |
| *optional* | *optional* | **`parent-snapshot-id`** | The snapshot ID of the snapshot’s parent. Omitted for any snapshot with no parent |
|            | *required* | **`sequence-number`**    | A monotonically increasing long that tracks the order of changes to a table |
| *required* | *required* | **`timestamp-ms`**       | A timestamp when the snapshot was created, used for garbage collection and table inspection |
| *optional* | *required* | **`manifest-list`**      | The location of a manifest list for this snapshot that tracks manifest files with additional meadata |
| *optional* |            | **`manifests`**          | A list of manifest file locations. Must be omitted if `manifest-list` is present |
| *optional* | *required* | **`summary`**            | A string map that summarizes the snapshot changes, including `operation` (see below) |

summary中的operation中可选为：

- **append**：添加新的data文件，没有文件删除；
- **replace**：添加或删除数据和删除文件，但是表的数据没有改动，如Compaction。
- **overwrite**：添加或删除数据和删除文件，逻辑上的覆盖；
- **delete**：移除数据文件，添加删除文件。

**快照的数据和删除文件可以存储在多个清单**中。这使得:

- 快速追加（Fast Appending）：追加可以添加一个新的清单来最小化写入的数据量，而不是通过重写和追加到现有清单来添加新记录。
- 表可以使用多个分区规范。如果表的数据卷发生变化，那么它的分区配置可能会发生变化。每个清单使用单个分区规范，查询不需要更改，因为分区筛选器是从数据谓词派生的。
- 大型表可以跨多个清单分割，以便实现可以并行化作业计划或减少重写清单的成本。

快照的清单由清单列表跟踪。有效的快照以列表形式存储在表元数据中。

#### 清单列表

**快照嵌入在表元数据中，但是快照的清单列表存储在单独的清单列表文件**中。

每次尝试提交快照都会写入新的清单列表，因为清单列表总是会更改以生成新的快照。当写入清单列表时，将为该列表跟踪的所有新清单文件写入快照的(乐观的)序列号。

**清单列表包含摘要元数据，可用于在规划表扫描时避免扫描快照中的所有清单**。这包括添加的、现有的和删除的文件的数量，以及用于写入清单的分区规范的每个字段的值的摘要。

清单列表是一个有效的Iceberg数据文件：文件必须使用有效的Iceberg格式、模式和列投影。

清单列表文件中存储`manifest_file`结构：

| v1         | v2         | Field id, name                 | Type                                        | Description                                                  |
| :--------- | :--------- | :----------------------------- | :------------------------------------------ | :----------------------------------------------------------- |
| *required* | *required* | **`500 manifest_path`**        | `string`                                    | Location of the manifest file                                |
| *required* | *required* | **`501 manifest_length`**      | `long`                                      | Length of the manifest file                                  |
| *required* | *required* | **`502 partition_spec_id`**    | `int`                                       | ID of a partition spec used to write the manifest; must be listed in table metadata `partition-specs` |
|            | *required* | **`517 content`**              | `int` with meaning: `0: data`, `1: deletes` | The type of files tracked by the manifest, either data or delete files; 0 for all v1 manifests |
|            | *required* | **`515 sequence_number`**      | `long`                                      | The sequence number when the manifest was added to the table; use 0 when reading v1 manifest lists |
|            | *required* | **`516 min_sequence_number`**  | `long`                                      | The minimum sequence number of all data or delete files in the manifest; use 0 when reading v1 manifest lists |
| *required* | *required* | **`503 added_snapshot_id`**    | `long`                                      | ID of the snapshot where the manifest file was added         |
| *optional* | *required* | **`504 added_files_count`**    | `int`                                       | Number of entries in the manifest that have status `ADDED` (1), when `null` this is assumed to be non-zero |
| *optional* | *required* | **`505 existing_files_count`** | `int`                                       | Number of entries in the manifest that have status `EXISTING` (0), when `null` this is assumed to be non-zero |
| *optional* | *required* | **`506 deleted_files_count`**  | `int`                                       | Number of entries in the manifest that have status `DELETED` (2), when `null` this is assumed to be non-zero |
| *optional* | *required* | **`512 added_rows_count`**     | `long`                                      | Number of rows in all of files in the manifest that have status `ADDED`, when `null` this is assumed to be non-zero |
| *optional* | *required* | **`513 existing_rows_count`**  | `long`                                      | Number of rows in all of files in the manifest that have status `EXISTING`, when `null` this is assumed to be non-zero |
| *optional* | *required* | **`514 deleted_rows_count`**   | `long`                                      | Number of rows in all of files in the manifest that have status `DELETED`, when `null` this is assumed to be non-zero |
| *optional* | *optional* | **`507 partitions`**           | `list<508: field_summary>` (see below)      | A list of field summaries for each partition field in the spec. Each field in the list corresponds to a field in the manifest file’s partition spec. |

`field_summary`结构：

| v1         | v2         | Field id, name           | Type        | Description                                                  |
| :--------- | :--------- | :----------------------- | :---------- | :----------------------------------------------------------- |
| *required* | *required* | **`509 contains_null`**  | `boolean`   | Whether the manifest contains at least one partition with a null value for the field |
|            | *optional* | **`518 containers_nan`** | `boolean`   | Whether the manifest contains at least one partition with a nan value for the field |
| *optional* | *optional* | **`510 lower_bound`**    | `bytes` [1] | Lower bound for the non-null, non-NaN values in the partition field, or null if all values are null or NaN [2] |
| *optional* | *optional* | **`511 upper_bound`**    | `bytes` [1] | Upper bound for the non-null, non-NaN values in the partition field, or null if all values are null or NaN [2] |

#### 扫描计划（Scan Planning）

扫描是通过读取当前快照的清单文件进行计划。在扫描中不会使用数据和删除清单中的已删除条目。

对于每个清单，**用于筛选数据行的扫描谓词被转换为用于筛选数据和删除文件的分区谓词**。这些分区谓词用于选择清单中的数据和删除文件。

**匹配查询筛选器的删除文件必须在读取时应用于数据文件**，使用以下规则限制：

- 位置删除文件必须应用于数据文件，当以下所有选项都为真时：

  - 数据文件的序号**小于或等于**删除文件的序号；

  - 数据文件的分区(分区规格和分区值)等于删除文件的分区；
- 相等删除文件应用于数据文件，当以下所有项都为真时：
  - 数据文件的序号**严格小于**删除文件的序号
  - 数据文件的分区(规格和分区值)等于删除文件的分区或删除文件的分区规格为未分区

一般来说，删除只适用于旧的数据文件，并且在同一个分区中，除了两种特殊情况:

- 没有分区规范存储的相等删除文件作为全局删除应用。否则，“删除文件”不适用于其他分区中的文件。
- 位置删除文件必须应用于来自同一提交的数据文件，当数据和删除文件序列号相等时。这允许删除在同一提交中添加的行。

### 表元数据

表的元数据以 JSON 存储。每个表的元数据改变时，会创建新的表元素据文件（通过原子性操作提交）。原子性操作确保表元数据的新版本替换它所基于的版本。这将**生成表版本的线性历史记录，并确保并发写操作不会丢失**。

**用于提交元数据的原子操作取决于如何跟踪表，该规范没有对其进行标准化**。

#### 表元数据字段

| v1         | v2         | Field                       | Description                                                  |
| :--------- | :--------- | :-------------------------- | :----------------------------------------------------------- |
| *required* | *required* | **`format-version`**        | An integer version number for the format. Currently, this is always 1. Implementations must throw an exception if a table’s version is higher than the supported version. |
| *optional* | *required* | **`table-uuid`**            | A UUID that identifies the table, generated when the table is created. Implementations must throw an exception if a table’s UUID does not match the expected UUID after refreshing metadata. |
| *required* | *required* | **`location`**              | The table’s base location. This is used by writers to determine where to store data files, manifest files, and table metadata files. |
|            | *required* | **`last-sequence-number`**  | The table’s highest assigned sequence number, a monotonically increasing long that tracks the order of snapshots in a table. |
| *required* | *required* | **`last-updated-ms`**       | Timestamp in milliseconds from the unix epoch when the table was last updated. Each table metadata file should update this field just before writing. |
| *required* | *required* | **`last-column-id`**        | An integer; the highest assigned column ID for the table. This is used to ensure columns are always assigned an unused ID when evolving schemas. |
| *required* | *required* | **`schema`**                | The table’s current schema.                                  |
| *required* |            | **`partition-spec`**        | The table’s current partition spec, stored as only fields. Note that this is used by writers to partition data, but is not used when reading because reads use the specs stored in manifest files. (**Deprecated**: use `partition-specs` and `default-spec-id`instead ) |
| *optional* | *required* | **`partition-specs`**       | A list of partition specs, stored as full partition spec objects. |
| *optional* | *required* | **`default-spec-id`**       | ID of the “current” spec that writers should use by default. |
| *optional* | *optional* | **`properties`**            | A string to string map of table properties. This is used to control settings that affect reading and writing and is not intended to be used for arbitrary metadata. For example, `commit.retry.num-retries` is used to control the number of commit retries. |
| *optional* | *optional* | **`current-snapshot-id`**   | `long` ID of the current table snapshot.                     |
| *optional* | *optional* | **`snapshots`**             | A list of valid snapshots. Valid snapshots are snapshots for which all data files exist in the file system. A data file must not be deleted from the file system until the last snapshot in which it was listed is garbage collected. |
| *optional* | *optional* | **`snapshot-log`**          | A list (optional) of timestamp and snapshot ID pairs that encodes changes to the current snapshot for the table. Each time the current-snapshot-id is changed, a new entry should be added with the last-updated-ms and the new current-snapshot-id. When snapshots are expired from the list of valid snapshots, all entries before a snapshot that has expired should be removed. |
| *optional* | *required* | **`sort-orders`**           | A list of sort orders, stored as full sort order objects.    |
| *optional* | *required* | **`default-sort-order-id`** | Default sort order id of the table. Note that this could be used by writers, but is not used when reading because reads use the specs stored in manifest files. |

#### 提交冲突解决和重试

当两个提交同时发生并且基于相同的版本时，只有一个提交会成功。在大多数情况下，失败的提交可以应用到表元数据的新当前版本并重试。更新将验证应用于新版本的条件，如果满足这些条件，则重试。

- Add操作没有要求，可以始终应用；
- Replace操作必须验证将要删除的文件仍在表中。替换操作的示例包括格式更改(将Avro文件替换为Parquet文件)和压缩(将多个文件替换为包含相同行的单个文件)；
- Delete操作必须验证要删除的特定文件仍然在表中。可以始终应用基于表达式的删除操作(例如，timestamp< X)；
- 表模式更新和分区规范更改必须验证基本版本和当前版本之间的模式没有更改。

#### 文件系统表（File System Tables）

原子性的交换操作可以通过**文件系统的原子性rename操作实现**，比如HDFS或者大多数的本地文件系统。文件系统表的schema通过`HadoopTableOperations`实现。

表元数据的每个版本存储在元数据的文件夹下（在表的基本路径下），通过版本号V限定名称为`v<V>.metadata.json`。提交新的元数据版本时，`V+1`，写入者需要执行以下步骤：

1. 读取当前表的元数据版本`V`；
2. 基于版本`V`创建新的表的元数据；
3. 根据表的新的元数据生成唯一的文件：`<random-uuid>.metadata.json`；
4. 重命名文件为`v<V+1>.metadata.json`，如果重命名成功，则提交成功；失败则跳到第一步。

#### 元数据表（Metastore Tables）

提交新版本的表元数据所需的原子交换可以通过**在元存储或数据库中存储一个指针来实现**，该数据库使用check-and-put操作进行更新。

表元数据的每个版本存储在元数据的文件夹下（在表的基本路径下），文件名格式为`<V>-<uuid>.metadata.json`，包含版本号和UUID。步骤如下：

1. 读取当前表的元数据版本`V`；
2. 基于版本`V`创建新的表的元数据；
3. 根据表的新的元数据生成唯一的文件：`<V+1>-<uuid>.metadata.json`；
4. 请求元存储将表的元数据指针从V的位置交换到V+1的位置，如果交换成功，则提交成功；失败则跳到第一步。

### 删除的格式

以下描述删除文件中对行级删除进行编码。**v1中不支持行级删除**。

**行级删除文件是有效的Iceberg数据文件**：文件必须使用有效的Iceberg格式、模式和列投影。建议使用表的默认文件格式写入删除文件。

行级删除文件由清单(如同数据文件一样)跟踪。删除文件使用一组单独的清单，但清单模式是相同的。

**位置删除和相等删除都允许用delete对已删除的行值进行编码**。这可用于重建表的更改流。

#### 位置删除文件

基于位置的删除文件通过一个或多个数据文件中的**文件和位置标识已删除的行**，并且可以选择包含已删除的行。

如果在位置删除文件中有一条记录是数据文件中行的文件和位置（从0开始），则删除数据行。

删除文件存储结构体`file_position_delete`：

| Field id          | name            | Type                   | Description                                                  |
| :---------------- | --------------- | :--------------------- | :----------------------------------------------------------- |
| **`2147483546`**  | **`file_path`** | `string`               | Full URI of a data file with FS scheme. This must match the `file_path` of the target data file in a manifest entry |
| **`2147483545`**  | **`pos`**       | `long`                 | Ordinal position of a deleted row in the target data file identified by `file_path`, starting at `0` |
| **`2147483544 `** | **`row`**       | `required struct<...>` | Deleted row values. Omit the column when not storing deleted rows. |

当删除的行列出现时，它的模式可以是表模式的任何子集，并且必须使用与表匹配的字段id。

**为了确保统计数据的准确性，所有的删除条目必须包括行值，或者必须省略列**(这就是为什么列类型必须是`required`)。

**删除文件中的行必须按file_path和position排序**，以便在扫描时优化过滤行。

- 按file_path排序允许在列示存储格式的文件下推过滤器。

- 按位置排序允许在扫描时过滤行，以避免在内存中保留删除。

#### 相等删除文件（？）

相等性删除文件通过一个或多个列值标识数据文件集合中已删除的行，并且可以选择包含已删除行的附加列。

相等删除文件存储表列的任何子集，并使用表的字段id。删除列是用于匹配数据行的删除文件中的列。删除列由删除文件元数据列`equality_ids`中的id标识。

**如果数据行的值等于应用于该行数据文件的相等删除文件中任何一行的所有删除列**，则删除该数据行。

delete文件的每一行产生一个相等谓词，该谓词匹配任何delete列相等的行。可以将多个列视为相等谓词的与。如果一行的值为空，则delete列中的空值与该行匹配，相当于`col is null`。

如果相等删除文件中的删除列稍后从表中删除，则在应用相等删除时仍必须使用该列。如果一个列被添加到一个表中，然后用作相等删除文件中的删除列，则使用普通投影规则读取旧数据文件的列值（默认为null）。

#### 删除文件统计

清单对删除文件和数据文件持有相同的统计信息。对于删除文件，指标描述了被删除的值。