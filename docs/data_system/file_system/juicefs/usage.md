# 使用

## 配置

### 元数据存储 

#### PostgreSQL

- 默认使用的 public [schema](https://www.postgresql.org/docs/current/ddl-schemas.html) 
- 密码中的特殊字符需要进行 url 编码，例如 `|` 需要编码为`%7C`

```shell
# 需要提前手动创建数据库，使用如下的格式来指定参数
export META_PASSWORD="mypassword"
juicefs format \
    --storage s3 \
    ... \
    "postgres://user@192.168.1.6:5432/juicefs" \
    pics
# 挂载文件系统
export META_PASSWORD="mypassword"
juicefs mount -d "postgres://user:mypassword@192.168.1.6:5432/juicefs" /mnt/jfs
```



#### Redis

> 支持使用 Redis Cluster 作为元数据引擎，但为了避免在 Redis 集群中执行跨节点事务，**同一个文件系统的元数据总会坐落于单个 Redis 实例**中。

保证元数据安全，参考 [Redis 最佳实践](https://juicefs.com/docs/zh/community/redis_best_practices)

-  内存满时不驱除：设置 [`maxmemory-policy noeviction`](https://redis.io/docs/reference/eviction/)，juicefs 启动时会自动设置，设置失败会打印警告；

- Redis需要开启持久化（RDB 和AOF），防止元数据丢失；

### 对象存储





### Hadoop 使用

> [在 Hadoop 生态使用 JuiceFS | JuiceFS Document Center](https://juicefs.com/docs/zh/community/hadoop_java_sdk#部署客户端)



## 命令

### juicefs auth

文件系统的认证授权。可以在挂载时以命令行交互方式或者参数方式输入认证信息，也可以用 `auth` 命令将认证信息保存在本地（位置是 `$HOME/.juicefs/[NAME].conf`），以后挂载时无需再次输入，这样更方便做自动化的运维配置。

### juicefs mount

> 挂载完成之后，默认会切到后端运行。

#### FUSE 挂载选项

> [FUSE 挂载选项 | JuiceFS Document Center](https://juicefs.com/docs/zh/community/fuse_mount_options)

```shell
# -o 选项设置
juicefs mount -d -o allow_other,writeback_cache sqlite3://myjfs.db ~/jfs
```



#### 缓存相关参数

```ini
--cache-dir=<cache_dir>
```

本地缓存的存储路径，默认是 `/var/jfsCache`。

多盘缓存用 `:` 分割不同目录，目录可包含通配符 `*` （需要添加引号）。

例如 `--cache-dir '/data*/jfsCache:/mydata*/jfsCache'`。

```ini
--cache-size=<cache_size>
```

本地缓存容量，默认是 1024 MiB。

在多盘缓存下是指多个目录一起的总容量，会平均分配到各个目录。

```ini
--free-space-ratio=<free_space_ratio>
```

缓存盘的最少剩余空间，默认是 0.2。

```ini
--cache-mode=<cache_mode>
```

磁盘上缓存文件的权限模式，默认是 0600。

```ini
--buffer-size=<buffer_size>
```

用来缓存读写数据的总内存大小，默认 300 MiB。

```ini
--writeback
```

写数据时优先写到本地磁盘，然后在后台异步上传到对象存，默认不启用。

```ini
--metacache
```

将元数据缓存在运行客户端的主机的内存里，默认启用，请用 –metacacheto=0 来关闭。

```ini
--metacacheto=<metacacheto>
```

元数据的缓存过期时间(单位是秒），默认为 300（5 分钟）。

```ini
--entrycacheto=<entrycacheto>
```

文件项在内核中的缓存时间，默认 1 秒。

```ini
--direntrycacheto=<direntrycacheto>
```

目录项在内核中的缓存时间，默认 1 秒。

```ini
--attrcacheto=<attrcacheto>
```

文件/目录的属性在内核中的缓存时间，默认 1 秒。

```ini
--opencache
```

是否使用缓存的元数据来打开文件，默认不使用。

### juicefs umount

卸载 JuiceFS 文件系统。

### juicefs info

显示 JuiceFS 文件系统中文件或目录的信息。

### juicefs import

从已有的对象存储中导入文件。

### juicefs rmr

快速删除目录里的所有文件和子目录。

### juicefs snapshot

创建或删除快照。

### juicefs grep

### juicefs lsof

列出 JuiceFS 最近（10 分钟内）被打开过的文件。

### juicefs profile

收集和分析文件系统的 oplog (operation log)。