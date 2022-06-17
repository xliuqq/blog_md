# Hive

## 架构



## 组件

### HiveServer2

通过Thrift RPC 提供服务端能力，支持多用户并发和安全认证。用户可以通过hiveserver2执行SQL语句获得返回结果。

### MetaStore

元数据包含用Hive创建的database、table等的元信息。元数据存储在关系型数据库中。如Derby、MySQL等。

**Embedded Metastore Database (Derby) 内嵌模式**

默认配置，只能一次启动一个客户端，**hive服务和metastore服务运行在同一个进程中**，默认数据存储到`./metastore_db`目录下。

**Local Metastore Server 本地元存储**

hive服务和metastore服务运行在同一个进程中，连接第三方的数据库（MySQL等）。

**Remote Metastore Server 远程元存储**

hive服务和metastore服务不运行在同一个进程中，连接第三方的数据库（MySQL等）。

### HCatalog

Hadoop的表存储管理工具，将Hive Metastore的表格数据公开给其它的数据处理工具。

HCatalog的表为用户提供了（HDFS）中数据的关系视图，并确保用户不必担心他们的数据存储在何处或采用何种格式（不需要修改客户端源码进行读写）。

#### *WebHCat* 

HCatalog的REST API。



## 表的性质

### 内部表和外部表

未被external修饰的是内部表（managed table），被external修饰的为外部表（external table）； 

区别： 

- 内部表数据由Hive自身管理，外部表数据由HDFS管理； 
- 内部表数据存储的位置是hive.metastore.warehouse.dir（默认：/user/hive/warehouse），外部表数据的存储位置由自己制定；     
- 删除内部表会直接删除元数据（metadata）及存储数据；删除外部表仅仅会删除元数据，HDFS上的文件并不会被删除；     
- 对内部表的修改会将修改直接同步给元数据，而**对外部表的表结构和分区进行修改，则需要修复**（MSCK     REPAIR TABLE table_name;）

### 分区表

表可以进一步被分区（Partitioned By），分成多级子目录，避免无需的查找。

### 分桶表

表和分区都可以被分桶（CLUSTERED BY columns），桶里的数据可以通过 SORT BY 排序。

```sql
create tablebucketed_user(id int,name string) clustered by (id) sorted by(name) into 4 buckets row format delimited fields terminated by ' ' stored as textfile; 
```



## Hive 运行模式

./bin/hive  --service metastore`

hive客户端在hive-site.xml配置hive.metastore.uris=thrift://ipxxx:9083  的方式链接到hive。motestore 虽然能使hive服务端和客户端分别部署到不同的节点，客户端不需要关注metadata的相关配置。但是metastore只能通过只能通过配置hive.metastore.uris的方式连接，无法通过jdbc的方式访问。

`./bin/hiveserver2`
hiveserver2 会启动一个hive服务端默认端口为：10000，可以通过beeline，jdbc，odbc的方式链接到hive。hiveserver2启动的时候会先检查有没有配置hive.metastore.uris，如果没有会先启动一个metastore服务，然后在启动hiveserver2。如果有配置hive.metastore.uris。会连接到远程的metastore服务。

