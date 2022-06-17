[toc]

# ColumnIndex 

Column Index pages 存储在 Parquet footer中。



## 问题描述

在以前版本中，column chunk的统计信息存储在ColumnMetaData中，每页的统计信息存储在DataPageHeader结构体中。当读取页时，reader必须处理page header，以确定是否可以根据统计数据跳过该页。这意味着**reader必须访问列中的所有页，因此可能从磁盘读取大部分列数据。**



## 目标

- Range scan或者单点查询 IO 更有效，可以根据最小/最大值直接访问页：
- 在基于**行组的排序列的行组中进行单行查找时，对于每个检索到的列只读取一个数据页**：
  - 对排序列的范围扫描只需要读取包含相关数据的确切数据页；
  - 提高其他选择性扫描I/O效率:如果我们在一个非排序列上有一个选择性的谓词，那么对于其他检索到的列，我们应该只需要访问包含匹配行的数据页。
- 没有选择性谓词的扫描无需额外的解码工作，例如全行组扫描。如果reader确定不需要读取索引数据，则不会产生任何开销。

- 通过只**存储页面之间的边界元素**，使得排序列的index pages使用了最小的存储空间。

## 非目标

- 支持二级索引，即根据键值对未排序的数据进行排序的索引结构。

  

## 技术方法

对每列增加两种数据结构，存储在row group metadata中：

- `ColumnIndex`：这允许根据列值导航到列的页，并用于定位包含扫描谓词匹配值的数据页；
- `OffsetIndex`：允许按行索引进行导航，用于通过ColumnIndex检索标识为匹配的行的值。跳过一列中的行后，必须跳过其他列中的相应行。因此，RowGroup中每个列的OffsetIndexes都存储在一起。

新的索引结构与RowGroup分开存储，位于footer附近。

这样做是为了让reader不必为读取它们而支付I/O和反序列化成本，如果它没有进行选择性扫描的话。

索引结构的位置和长度存储在ColumnChunk中。

![Page Index Layout](pics/columnindex.png)

- 不需要记录第一个页的下界和最后一个页的上界，因为行组统计信息可以提供这一点。为了保持一致性，我们仍然包括这些，开销应该可以忽略不计。
- 为每个页面的值存储下界和上界。这些值可以是页面上实际找到的最小值和最大值，但也可以是页面上不存在的值。例如，写入器可以设置min_values[i]="B"， max_values[i]="C"，而不是存储""Blart Versenwald III"。这允许写入器截断较大的值，写入器应该使用它对索引结构的大小实施一些合理的限制。
- 支持columnIndex的Readers不要使用page statistics.。在写columnIndex时写page=level统计信息，仅仅是为了支持旧版本的reader。



对于有序的列，这允许reader通过在min_values和max_values中执行二分搜索来查找匹配的页。

对于无序列，reader可以通过顺序读取min_values和max_values来找到匹配的页。

对于范围扫描，这种方法可以扩展为返回要扫描的每列中的行、页索引和页偏移量的范围。然后，读取器可以为每列初始化一个扫描器，并将它们快进到扫描的起始行。