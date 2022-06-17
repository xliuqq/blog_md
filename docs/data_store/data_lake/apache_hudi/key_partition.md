# 主键和分区配置

HoodieKey由`记录键`和记录所属的`分区路径`组成。

- 将更新和删除快速应用于指定记录；
- Hudi使用`分区路径`字段对数据集进行分区，并且分区内的记录有唯一的记录键；
- 仅在分区内保证唯一性，不同分区之间可能存在具有相同记录键的记录。应该明智地选择分区字段，因为它可能影响摄取和查询延迟。



## 键生成器

### 类型

- *SimpleKeyGenerator*
  - 指定**一个字段为Record Key，分区字段也指定一个字段**，大部分情况使用该配置，其会从数据中解析出值并转化为string类型。
- *ComplexKeyGenerator*
  - 可以指定**一个或多个字段作为Record key或者分区字段**，多个字段使用逗号分割；
- *GlobalDeleteKeyGenerator*
  - 基于全局索引的删除不需要分区值，所以该生成器**不需要使用分区值来生成HoodieKey**。
- *TimestampBasedKeyGenerator*
  - 该键生成器将分区字段值会被转化为`timestamp`，而不是string类型。Record key设置和前面一样，使用该键生成器时需要一些额外的配置项如下
- *CustomKeyGenerator*
  - 通用的KeyGenerator，综合了`SimpleKeyGenerator`、`ComplexKeyGenerator`和`TImestampBasedKeyGenerator`的特性，可以配置键和分区路径为单字段或组合字段
  - 如果要定义基于常规字段和基于时间戳的字段的复杂分区路径，此keyGenerator非常有用
- *NonPartitionedKeyGenerator*
  - 数据集未分区，则可以使用*NonPartitionedKeyGenerator*，**为所有记录返回一个空分区**。换句话说，所有记录都写入到同一个分区（为空""）

### 配置

| 配置 | 解释 |
| :------------------------------------------------ | :----------------------------------------------------------: |
| `hoodie.datasource.write.recordkey.field` | 指定record key字段 |
| `hoodie.datasource.write.partitionpath.field` | 默认N/A，指定分区字段 |
| `hoodie.datasource.write.keygenerator.class` | 指定KeyGenerator类全路径名 |
| `hoodie.datasource.write.partitionpath.urlencode` | 当设置为true，partitionPath将会使用url编码，默认值为false |
| `hoodie.datasource.write.hive_style_partitioning` | 当设置为true，使用hive风格的分区，分区将为key=value格式，默认值为false |
| `hoodie.datasource.write.keygenerator.type` | 默认SIMPLE，支持`SIMPLE, COMPLEX, TIMESTAMP, CUSTOM, NON_PARTITION, GLOBAL_DELETE` |



bootstrap：控制如何将现有的表第一次引导到 hudi。

- `hoodie.bootstrap.keygen.type`：默认SIMPLE，支持`SIMPLE, COMPLEX, TIMESTAMP, CUSTOM, NON_PARTITION, GLOBAL_DELETE`；
- `hoodie.bootstrap.keygen.class`：默认N/A，自定义bootstrapped dataset的hoodie key的生成策略；

