# 分布式SQL引擎 

## 运行Thrift的JDBC/ODBC 服务

```shell
./sbin/start-thriftserver.sh \
  --hiveconf hive.server2.thrift.port=<listening-port> \
  --hiveconf hive.server2.thrift.bind.host=<listening-host> \
  --master <master-uri>
  ...
```




## 运行Spark SQL CLI

> ./bin/spark-sql

Spark SQL CLI不能和Thrift JDBC服务器连接。

以本地模式运行Hive metastore，从命令行执行查询语句。

Hive的配置hive-site.xml，core-site.xml和hdfs-site.xml需要放到spark的配置conf目录。