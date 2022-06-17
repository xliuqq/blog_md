[toc]

# HDFS中的集中式缓存管理

显式缓存机制，允许用户**指定HDFS要缓存的路径**。NameNode将与磁盘上具有所需块的数据节点通信，并指示它们将**块缓存在堆外缓存**中。

## 架构

<img src="pics/caching.png" alt="Caching Architecture" style="zoom:80%;" />

NameNode负责协调集群中所有DataNode的堆外缓存。NameNode**定期从每个DataNode接收一个缓存报告**，该报告描述给定DN上缓存的所有块。NameNode通过在DataNode **heartbeat上搭载cache和uncache命令**来管理DataNode缓存。

NameNode查询其**缓存指令以确定应缓存哪些路径**。缓存指令**持久存储在fsimage和edit log**中，可以通过Java和命令行api添加、删除和修改。

NameNode还存储一组缓存池，用于**将缓存指令组合在一起以进行资源管理和强制执行权限**的管理实体。

NameNode**定期重新扫描命名空间和活动缓存指令**，以确定哪些块需要缓存或取消缓存，并将缓存工作分配给DataNodes。**重新扫描也可以由用户操作触发**，如添加或删除缓存指令或删除缓存池。

如果cache指令覆盖symlink，则**不缓存symlink目标**。

**缓存当前在文件或目录级别完成**，当前版本不支持单独缓存块和子块。

## 概念

### 缓存指令

*cache directive* ：定义被缓存的路径（文件/目录）。目录只会缓存以及子文件，不会递归缓存；

- 可以指定缓存副本因子数和过期时间；

### 缓存池

缓存池是用于**管理缓存指令组的管理实体**。缓存池具有类似UNIX的权限，**写入权限**允许用户向池中添加和删除缓存指令；读取权限允许用户列出池中的缓存指令以及其他元数据；执行权限未使用。

缓存池也用于**资源管理**，限制可以通过池中的所有指令可以缓存的字节数。

缓存池可以指定**最长生存时间**，限制添加到池中的指令的最长过期时间。

## CLI

### Cache directive commands

#### addDirective

添加缓存指令

` hdfs cacheadmin -addDirective -path <path> -pool <pool-name> [-force] [-replication <replication>] [-ttl <time-to-live>]`

- `-force`：跳过缓存池的资源限制的检查；

#### removeDirective

**移除单个缓存指令**

Usage: `hdfs cacheadmin -removeDirective  <id>`

#### removeDirectives

**移除特定路径关联的每个缓存指令**

Usage: `hdfs cacheadmin -removeDirectives <path>`

#### listDirectives

Usage: `hdfs cacheadmin -listDirectives [-stats] [-path <path>] [-pool <pool>]`

### Cache pool commands

#### addPool

`hdfs cacheadmin -addPool <name> [-owner <owner>] [-group <group>] [-mode <mode>] [-limit <limit>] [-maxTtl <maxTtl>]`

- `limit`：最大缓存字节`

#### modifyPool

`hdfs cacheadmin -modifyPool <name> [-owner <owner>] [-group <group>] [-mode <mode>] [-limit <limit>] [-maxTtl <maxTtl>]`

#### removePool

`hdfs cacheadmin -removePool <name>`

- 会取消与池关联的路径的缓存

#### listPools

 `hdfs cacheadmin -listPools [-stats] [<name>]`



## 配置

### dfs.datanode.max.locked.memory

DataNode可以缓存的最大内存，对于Unix-like系统，需要设置DataNode用户的**“locked-in-memory size” ulimit (`ulimit -l`)**，提高到匹配该值。

### dfs.datanode.pmem.cache.dirs

缓存的路径，如 /mnt/pmem0, /mnt/pmem1，如果配置这个则不需要配置`dfs.datanode.max.locked.memory`





## 可选

- dfs.namenode.path.based.cache.refresh.interval.ms
  - 默认30000，路径缓存重新扫描（指计算应缓存哪些块以及在哪些数据节点上进行缓存）的间隔。

- dfs.datanode.fsdatasetcache.max.threads.per.volume
  - 默认4，DataNode将使用此值作为每个卷用于缓存新数据的最大线程数。

- dfs.cachereport.intervalMsec
  - 默认10000，DataNode将使用此值作为向NameNode发送其缓存状态的完整报告之间的毫秒数。

- dfs.namenode.path.based.cache.block.map.allocation.percent
  - 默认0.25, 分配给缓存块映射（cached block map）的Java堆的百分比。缓存块映射是使用链式哈希的哈希映射。

- dfs.namenode.caching.enabled
  - 该参数可用于开启/关闭NameNode的集中缓存。当禁用集中缓存时，NameNode将不会处理缓存报告或在集群中存储块缓存位置信息。请注意，NameNode将继续在文件系统元数据中存储基于路径的缓存位置，即使在启用缓存之前它不会对这些信息采取行动。该参数的默认值为true(即启用集中式缓存)。
- dfs.datanode.pmem.cache.recovery
  - 判断是否在启动DataNode时恢复持久化内存上的前一个cache的状态。如果启用，DataNode将恢复持久化内存中先前缓存的数据的状态。因此，将避免重新缓存数据。如果未启用此属性，则DataNode将清理持久内存上的前一个缓存(如果有的话)。

### OS限制

错误 “Cannot start datanode because the configured max locked memory size… is more than the datanode’s available RLIMIT_MEMLOCK ulimit,”

通过修改 ulimit -l 的值，允许锁定的最大内存配置。