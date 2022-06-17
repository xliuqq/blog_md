# KairosDB

KairosDB最初是从OpenTSDB 1.x版本fork出来的一个分支，使用Cassandra替换了HBase。

- 可插拔式的存储引擎

  - OpenTSDB异步的HBase Client（现在作为独立的一个开源项目输出：AsyncHBase）；

  - KairosDB严格定义了存储层的API Interface，整体逻辑与存储层耦合度较低，能比较容易的扩展多种存储引擎。
- 支持多种数据类型及自定义类型的值
  - OpenTSDB只支持numeric的值
  - KairosDB支持numeric、string类型的值，也支持自定义数值类型

- 不同的存储模型

  - OpenTSDB在存储模型上使用了UID的压缩优化，来优化查询和存储

  - KairosDB采取了一个不同的思路，利用了Cassandra宽表的特性



本质上，OpenTSDB存储模型采用的UID编码优化，主要解决的就两个问题：

- 存储空间优化：UID编码解决重复的row key存储造成的冗余的存储空间问题。
- 查询优化：利用UID编码后TagKey和TagValue固定字节长度的特性，利用HBase的FuzzyRowFilter做特定场景的查询优化。



KairosDB在解决这两个问题上，采取了另外一种不同的方式，使其不需要使用UID编码，也不存在使用UID编码后遗留的问题。先看下KairosDB的存储模型是怎样的，它主要由以下三张表构成：

1. DataPoints: 存储所有原始数据点，每个数据点也是由metric、tags、timestamp和value构成。该表中一行数据的时间跨度是三周，也就是说三周内的所有数据点都存储在同一行，而OpenTSDB内的行的时间跨度只有一个小时。RowKey的组成与OpenTSDB类似，结构为`tagv2>...`，不同的是metric, tag key和tag value都存储原始值，而不是UID。
2. RowKeyIndex: 该表存储所有metric对应DataPoints表内所有row key的映射，也就是说同一个metric上写入的所有的row key，都会存储在同一行内，并且按时间排序。该表主要被用于查询，在根据tag key或者tag value做过滤时，会先从这张表过滤出要查询的时间段内所有符合条件的row key，后在DataPoints表内查询数据。
3. StringIndex: 该表就三行数据，每一行分别存储所有的metric、tag key和tag value。



KairosDB内查询的整个流程：

1. 根据查询条件，找出所有DataPoints表里的row key
   - 如果有自定义的plugin，则从plugin中获取要查询的所有row key。（通过Plugin可以扩展使用外部索引系统来对row key进行索引，例如使用ElasticSearch）
   - 如果没有自定义的plugin，则在RowKeyIndex表里根据metric和时间范围，找出所有的row key。（根据列名的范围来缩小查询范围，列名的范围是(metric+startTime, metric+endTime))
2. 根据row key，从DataPoints表里找出所有的数据



相比OpenTSDB直接在数据表上进行扫描来过滤row key的方式，KairosDB利用索引表无疑会大大减少扫描的数据量。在metric下tagKey和tagValue组合有限的情况下，会大大的提高查询效率。