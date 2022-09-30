[toc]

# 配额

HDFS允许管理员设置**用于单个目录的名称数量和空间量的配额**。**名称配额和空间配额独立**。

**配额会持久化在FSImage中，并且设置会删除配额会创建journal entry。**

## 名称配额

名称配额是对根目录树中**文件名和目录名数量的硬限制**(hard limit)。

**目录重命名，其配额不变**。

**即使目录违反新配额，设置配额的尝试仍将成功。**

新创建的目录没有关联的配额。最大的配额是Long.Max值，**配额为1将强制目录保持为空**。

## 空间配额

空间配额是对根目录树中**文件使用的字节数**的硬限制。

块的每个副本都计算在配额内。

**目录重命名，其配额不变**。**目录不使用主机文件系统空间，也不计入空间配额。**

新创建的目录没有关联的配额。最大的配额是Long.Max值。**配额为零仍允许创建文件，但不能向文件中添加块**。

用于保存文件元数据的主机文件系统空间不计入配额。

## 空间类型配额

`dfs.quota.by.storage.type.enabled`默认true。

要在目录上设置存储类型配额，**必须在目录上[配置存储策略](storage_policy.md)**，以便根据存储策略将文件存储在不同的存储类型中。

可用的类型为： ***RAM_DISK***, ***DISK***, ***SSD***, ***ARCHIVE***。

存储类型配额可以与空间配额和名称配额相结合，以有效地管理群集存储使用情况。

## 命令

设置配额和清除配额

`hdfs dfsadmin -setQuota <N> <directory>...<directory>`

`hdfs dfsadmin -clrQuota <directory>...<directory>`

`hdfs dfsadmin -setSpaceQuota <N> <directory>...<directory>`

`hdfs dfsadmin -clrSpaceQuota <directory>...<directory>`

`hdfs dfsadmin -setSpaceQuota <N> -storageType <storagetype> <directory>...<directory>`

`hdfs dfsadmin -clrSpaceQuota -storageType <storagetype> <directory>...<directory>`



配额报告

`hadoop fs -count -q [-h] [-v] [-t [comma-separated list of storagetypes]] <directory>...<directory>`

- -q：显示每个目录的设置的名称配额，剩余的可用名称配额，设置的空间的配额值和可用的空间配额值

- -h：可读形式
- -v：显示标题行
- -t：显示每个存储类型的配额集和每个目录的剩余可用配额，可以指定显示的特定存储类型；