

# FAQ

## 运行

pyspark yarn-client 应用，task的执行时间为1h，结束后发现driver端和am连接异常；

信息：

> 20/01/08 17:00:55 ERROR client.TransportResponseHandler: Still have 1 requests outstanding when connection from /127.0.0.1:53680 is closed
> 20/01/08 17:00:55 ERROR cluster.YarnSchedulerBackend$YarnSchedulerEndpoint: Sending RequestExecutors(0,0,Map(),Set()) to AM was unsuccessful
> java.io.IOException: Connection from /127.0.0.1:53680 closed
>         at org.apache.spark.network.client.TransportResponseHandler.channelInactive(TransportResponseHandler.java:146)

定位：

- driver端k8s docker应用，tcp keepalive时间为10min；

- yarn am 端物理机，tcp keepalive时间为30min；

- TODO：原理？driver 端连接断掉，还未触发保活？

  网络连接拓扑：

  driver docker ->   ??  -> yarn am



## windows下开发

### 通过Intellij Idea运行spark程序，遇到文件读写时，报错 string: null chmod 0644

解决方案见[StackOverflow](https://stackoverflow.com/questions/40764807/null-entry-in-command-string-exception-in-saveastextfile-on-pyspark)。

将https://github.com/cdarlint/winutils中对应hadoop版本中的bin下的winutils下载到本地，并配置HADOOP_HOME，并将 `bin/`目录加入 `path`环境变量中；

### SparkSQL启动时，报The  root scratch dir: /tmp/hive on HDFS should be writable

解决方案，通过winutils修改权限（\tmp\hive目录是在磁盘驱动目录下的）。

```
%HADOOP_HOME%\bin\winutils.exe ls \tmp\hive
%HADOOP_HOME%\bin\winutils.exe chmod 777 \tmp\hive
%HADOOP_HOME%\bin\winutils.exe ls \tmp\hive
```

