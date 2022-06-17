# Presto 安装

PrestoDB的下载地址（PrestoSQL更名为Trino）：

- 下载 [presto-server.tar.gz](https://repo1.maven.org/maven2/com/facebook/presto/presto-server/0.261/presto-server-0.261.tar.gz)，解压后重命名为 presto-server；
- 在**同级目录创建data目录**，保存数据，方便presto-sever升级；

- 配置Presto，在presto-server目录下创建etc目录，配置：
  - Node Properties : 每个节点的环境配置
  - JVM Config : JVM配置
  - Config Properties : Presto server 配置
  - Catalog Properties : Connectors 配置

- 启动Presto
  - `bin/launcher start ` : deamon运行
  - `bin/launcher run` : 前台运行