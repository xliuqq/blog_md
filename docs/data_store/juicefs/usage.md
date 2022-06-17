# 使用

## 配置

### 支持的元数据存储

单台 Redis Server，目前不支持Redis cluster

- JuiceFS uses **[transaction](https://redis.io/topics/transactions) to guarantee the atomicity of metadata operations, which is not well supported in cluster mode**.

Redis需要开启持久化（RDB 和AOF），防止元数据丢失。

#### 支持的对象存储

- Amazon S3
- Google Cloud Storage
- Azure Blob Storage
- Alibaba Cloud Object Storage Service (OSS)
- Tencent Cloud Object Storage (COS)
- QingStor Object Storage
- Ceph RGW
- MinIO
- Local disk
- Redis

## 命令

### juicefs auth

文件系统的认证授权。可以在挂载时以命令行交互方式或者参数方式输入认证信息，也可以用 `auth` 命令将认证信息保存在本地（位置是 `$HOME/.juicefs/[NAME].conf`），以后挂载时无需再次输入，这样更方便做自动化的运维配置。

### juicefs mount

挂载完成之后，默认会切到后端运行。

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