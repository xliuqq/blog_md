# 安装部署

## 下载

- 下载相应版本的预编译包，解压，并配置环境变量(可选)

  ```bash
  export HIVE_HOME = 
  PATH=${HIVE_HOME}/bin:$PATH
  ```

- 创建相应的配置文件
  
  - `hive-env.sh, hive-site.xml, hive-log4j.properties`

## 依赖 

- Hadoop依赖
  - `hadoop`命令在`PATH`中；
  - 在`hive-env.sh`中配置`HADOOP_HOME`
- 存储依赖，HDFS（`chmod g+w`）
  - `/tmp`目录
  - `/user/hive/warehouse`目录（`hive.metastore.warehouse.dir`参数配置）

## 配置

- **hive.execution.mode**: 默认mr，改成 tez 或者 spark；

- **hive.server2.transport.mode** :  binary（默认） 或者 http

- **hive.server2.thrift.bind.host** 

- **hive.server2.thrift.port**


### metastore

- **javax.jdo.option.ConnectionURL**
- **javax.jdo.option.ConnectionDriverName**
- **hive.metastore.uris**
- **hive.metastore.warehouse.dir**

## 启动

### hiveserver2 & beeline

hiveserver2是个服务端（支持多用户、安全），beeline是个客户端。

```shell
$HIVE_HOME/bin/hiveserver2
$HIVE_HOME/bin/beeline -u jdbc:hive2://$HS2_HOST:$HS2_PORT
```

