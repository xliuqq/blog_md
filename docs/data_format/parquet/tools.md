# Tools

## parquet_tools

```xml
<!-- https://mvnrepository.com/artifact/org.apache.parquet/parquet-tools 
 2019-12 -->
<dependency>
    <groupId>org.apache.parquet</groupId>
    <artifactId>parquet-tools</artifactId>
    <version>1.11.0</version>
</dependency>
```

```shell
# Run from Java
java -jar ./parquet-tools-<VERSION>.jar <command> my_parquet_file.lzo.parquet
# Run from Hadoop
hadoop jar ./parquet-tools-<VERSION>.jar <command> my_parquet_file.lzo.parquet
```

支持的命令：

- merge，将多个parquet文件/目录合并成一个parquet文件；
- cat
- head
- size
- row_count
- meta
- dump
- ...

## Language

### Python

pyarrow 和 fastparquet 提供，pandas中dataframe选择其中之一实现 to_parquet 和 read_parquet。





