[toc]

# 流

Spark Structed Streaming 支持。

```sql
CREATE TABLE streaming_table (
 col1 INT,
 col2 STRING
)
STORED AS carbondata
TBLPROPERTIES('streaming'='true')
```

Carbon有两种DataSource（SPI机制）：

- carbon：文件格式，SparkCarbonFileFormat
- carbondata：表的格式，CarbonSource