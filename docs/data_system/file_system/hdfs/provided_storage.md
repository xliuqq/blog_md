[toc]

# HDFS提供式存储（experimental ）

> 通过HDFS能够定位到数据的外部存储。

在现有阶段，这个外部存储只支持读取操作，暂不支持外部存储的写。

在提供式存储的实现中，为了保持本身HDFS内部文件读取操作的逻辑与兼容性要求，需要做到以下两点：

- 远程文件系统（外部存储）需要实现Hadoop内部FileSystem接口。
- 提供式存储以新的Storage Type：PROVIDED类型被DN，NN所区别。就是说，当客户端访问的数据在PROVIDED类型的目录下时，表明其下的数据来自于外部存储。
- 元数据的映射：HDFS Block数据对于远程文件系统文件元数据的映射。

## 配置

```xml
<configuration>

  <property>
    <name>dfs.namenode.provided.enabled</name>
    <value>true</value>
    <description>Enabled provided storage on the Namenode</description>
  </property>

  <property>
     <name>dfs.datanode.data.dir</name>
     <value>[DISK]/local/path/to/blocks/, [PROVIDED]remoteFS://remoteFS-authority/path/to/data/</value>
  </property>

  <property>
      <name>dfs.provided.storage.id</name>
      <value>DS-PROVIDED</value>
      <description>The storage ID used for provided storages in the cluster.</description>
  </property>

  <property>
    <name>dfs.provided.aliasmap.class</name>
    <value>org.apache.hadoop.hdfs.server.common.blockaliasmap.impl.TextFileRegionAliasMap</value>
  </property>

</configuration>
```



aliasmap 目前只支持:

- InMemoryAliasMap
- TextFileRegionAliasMap