# jars

## Classpath isolation

Hadoop 3.0 防止不同版本jar包冲突，https://issues.apache.org/jira/browse/HADOOP-11656



## hadoop-client

同样的protobuf类，在client和hdfs中都会有：

- hadoop-client-api 中的proto 生成类，其Google Protobuf类会被shaded；

- 但是 hadoop-hdfs-api中的proto生成类，其Google Protobuf类不会被shaded；

