#  数据存储格式

像Apache Hive，Impala，Spark 这种数据分析引擎能够访问读取的文件格式。
- TextFile --- Hadoop TextInputFormat

  存储方式为行式存储，在检索时磁盘开销大，数据解析开销大，而对压缩的text文件，hive无法进行合并和拆分；

- SequcenFile --- Hadoop SequenceFileInputFormat

  二进制文件，以<key,value>的形式序列化到文件中，存储方式为行式存储，可以对文件进行分割和压缩，一般使用block压缩；

- Apache ORC-- Optimized Row Columnar
  根据行组分割整个表，在每一个行组内进行按列存储。ORC文件是自描述的，它的元数据使用Protocol Buffers序列化。对复杂数据类型特殊处理的方式实现嵌套格式的支持，ORC可以一定程度上支持ACID操作

- Apache Parquet -- Hadoop ParquetInputFormat
**列式存储**，文件中包括该文件的数据和元数据，Parquet支持嵌套的数据模型。



# 数据编码方式



见 [编码](./encoding.md)



# 数据压缩方式