[toc]

# 分布式索引服务

## 动机

carbon对于一般的表会修剪并缓存所有的Block/Blocklet的datamap到driver的内存中，对于Bloom/Index的datamaps，driver会启动job在executor端进行修剪和缓存。

driver会变成瓶颈：

- 缓存过大，导致GC严重，影响查询性能，且耗内存；
- LRU缓存替换，会减慢查询速度；
- 对于Bloom，不能保证请求到同一个executor上，导致多个executor上的缓存重复；
- 每个JDBC driver都需要单独的缓存。

## Distribution

对carbon表的查询都将以**请求的形式路由到索引服务器服务**。

IndexServer主要负责两件事：在executors间均分segment以及保存cache segment的位置。

- segment到executor的映射：每张表一个映射，记录每个segment cache的位置；
- 每个executor的cache大小：根据已经缓存的cache大小，在executors间均分segment；

**executor不可用时，基于负载重新划分其负责的segment。**

## 配置参数

### JDBCServer(carbon.properties)

| Name                                   | Default Value | Description                                                  |
| -------------------------------------- | ------------- | ------------------------------------------------------------ |
| carbon.enable.index.server             | false         | 应用级别的index server的启动与否                             |
| carbon.index.server.ip                 | NA            | index server ip                                              |
| carbon.index.server.port               | NA            | index server port                                            |
| carbon.disable.index.server.fallback   | false         | index server的fallback机制是否允许。应该仅用于测试目的。     |
| carbon.index.server.max.jobname.length | NA            | 在索引服务器服务UI中显示的作业的最大长度。对于较大的查询，这可能会影响性能，因为整个字符串将从JDBCServer发送到IndexServer。 |
| carbon.indexserver.enable.prepriming   | false         | 允许indexserver预执行提升第一次查询的性能                    |

### IndexServer(carbon.properties)

| Name                                                      | Default Value | Description                                                  |
| --------------------------------------------------------- | ------------- | ------------------------------------------------------------ |
| carbon.index.server.ip                                    | NA            | index server ip                                              |
| carbon.index.server.port                                  | NA            | index server port                                            |
| carbon.index.server.max.worker.threads                    | 500           | 要打开以接受来自JDBC驱动程序的请求的RPC处理程序的数量。      |
| carbon.max.executor.lru.cache.size                        | NA            | 执行程序进程可以缓存数据（DataMap和反向字典值）的最大内存**（以MB为单位**）。. |
| carbon.index.server.max.jobname.length                    | NA            | 在索引服务器应用程序UI中显示的作业的最大长度。               |
| carbon.max.executor.threads.for.block.pruning             | 4             | 用于块修剪的最大执行程序线程。                               |
| carbon.index.server.inmemory.serialization.threshold.inKB | 300           | 内存序列化大小达到阈值数据后数据将被写入文件。               |
| carbon.indexserver.temp.path                              | tablePath     | 超过阈值后文件写的临时路径                                   |

### Spark配置

#### spark-defaults.conf(安全模式)

| Name                            | Default Value | Description                                                  |
| ------------------------------- | ------------- | ------------------------------------------------------------ |
| spark.dynamicAllocation.enabled | true          | Set to false, so that spark does not kill the executor, If executors are killed, cache would be lost. Applicable only for Index Server. |
| spark.yarn.principal            | NA            | Should be set to the same user used for JDBCServer. Required only for IndexServer. |
| spark.yarn.keytab               | NA            | Should be set to the same as JDBCServer.                     |

#### spark-defaults.conf(非安全模式)

| Name                            | Default Value | Description                                                  |
| ------------------------------- | ------------- | ------------------------------------------------------------ |
| spark.dynamicAllocation.enabled | true          | Set to false, so that spark does not kill the executor, If executors are killed, cache would be lost. Applicable only for Index Server. |

**NOTE:** 最好为索引服务器主体创建一个新用户，该用户将对用户进行身份验证以访问索引服务器

#### core-site.xml

| Name                                       | Default Value | Description                                                  |
| ------------------------------------------ | ------------- | ------------------------------------------------------------ |
| ipc.client.rpc-timeout.ms                  | NA            | Set the above property to some appropriate value based on your estimated query time. The best option is to set this to the same value as spark.network.timeout. |
| hadoop.security.authorization              | false         | Property to enable the hadoop security which is required only on the server side. |
| hadoop.proxyuser.<indexserver_user>.users  | NA            | Property to set Proxy User list for which IndexServer permission were to be given ,check https://hadoop.apache.org/docs/current/hadoop-project-dist/hadoop-common/Superusers.html |
| hadoop.proxyuser.<indexserver_user>.hosts  | NA            | Property to set hosts list for which IndexServer permission were to be given ,check https://hadoop.apache.org/docs/current/hadoop-project-dist/hadoop-common/Superusers.html |
| hadoop.proxyuser.<indexserver_user>.groups | NA            | Property to set groups list for which IndexServer permission to be given ,check https://hadoop.apache.org/docs/current/hadoop-project-dist/hadoop-common/Superusers.html |
| security.indexserver.protocol.acl          | *             | Property to set List of User to be Authorized for Other than proxy Spark Application |

### 动态配置(set 命令)

| Name                                        | Default Value | Description                                                  |
| ------------------------------------------- | ------------- | ------------------------------------------------------------ |
| carbon.enable.index.server                  | false         | Enable the use of index server for pruning for the current session. |
| carbon.enable.index.server.dbName.tableName | false         | Enable the use of index server for the specified table in the current session. |

## 启动服务器

```shell
./bin/spark-submit --master [yarn/local] --[optional parameters] --class org.apache.carbondata.indexserver.IndexServer [path to carbondata-spark-<version>.jar]
```