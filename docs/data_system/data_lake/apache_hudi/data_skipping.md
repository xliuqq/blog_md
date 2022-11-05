# 数据跳过技术

> 参考：[Apache Hudi数据跳过技术加速查询高达50倍](https://mp.weixin.qq.com/s/IEDdmrYM3-eBS-rvqb0mWg)
>
> [1]  Z-order和希尔伯特空间填充曲线: *[https://hudi.apache.org/blog/2021/12/29/hudi-zorder-and-hilbert-space-filling-curves](https://hudi.apache.org/blog/2021/12/29/hudi-zorder-and-hilbert-space-filling-curves)*
> [2] 各种类型索引: *[https://arxiv.org/abs/2009.08150](https://arxiv.org/abs/2009.08150)*
>
> [3] Hudi 多模索引：https://www.onehouse.ai/blog/introducing-multi-modal-index-for-the-lakehouse-in-apache-hudi

## 背景

数据跳过本质上是各种类型索引$^{[2]}$的通用术语，使查询引擎能够有效地跳过数据，这与它当前执行的查询无关，以减少扫描和处理的数据量，节省扫描的数据量以及（ 潜在地）显着提高执行时间。

- 了解哪些文件可能包含查询正在寻找的数据，查询引擎必须读取表中每个 Parquet 文件的 Parquet 页脚，会影响性能；

## 发展历史

### Hudi 0.10

- 引入对高级数据布局优化技术的支持，例如 **Z-order和希尔伯特空间填充曲线**$^{[1]}$（作为新的聚类算法）；
- 引入简单的**列统计索引**（存储为简单的 Parquet 表），支持 Hudi 中数据跳过实现，展示 Z-order 和 Hilbert 的强大功能空间填充曲线作为高级布局优化技术；

### Hudi 0.11

Hudi 0.11 中，在元数据表中引入多模索引$^{[3]}$，例如布隆过滤器索引和列统计索引，作为**元数据表中的专用分区**（分别为“column_stats”和“bloom_filters”）：

- 列统计索引 (CSI) 享有元数据表的一致性保证；
- 元数据表使用 HBase的HFile作为基础文件和日志文件格式，促进基于键的快速查找（排序键值存储）；



### 下一步

[HUDI-1822](https://issues.apache.org/jira/browse/HUDI-1822)

- 支持 Merge-On-Read 表中的数据跳过
- 为列统计索引查询添加缓存
- 进一步分析和优化列统计索引性能



## 设计

> [RFC-27](*https://github.com/apache/hudi/tree/master/rfc/rfc-27](https://github.com/apache/hudi/tree/master/rfc/rfc-27*)



## 配置

摄取期间**同步**构建列统计索引：

- `hoodie.metadata.index.column.state.enbale=true`

异步索引器功能**回填**现有表的索引



查看数据跳过操作：默认情况下元数据表仅在写入端启用，需要以下配置

- `hoodie.metadata.enable=true`：确保在读取路径上启用了元数据表
- `hoodie.enable.data.skipping=true`：数据跳过功能已启用



