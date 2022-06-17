[toc]

## 存储类型

### ARCHIVE、DISK、SSD、RAM_DISK

异构存储，支持的存储类型 archive, disk, ssd 和 ram_disk。

archive：表示更大的存储但低的计算能力；

ram_disk：支持在内存中写单个副本文件。



## 存储策略

### Hot、Warm、Cold、ALL_SSD、Lazy_Persist、Provided

将文件根据存储策略存储为不同的存储类型。

- **Hot** - for both storage and compute. The data that is popular and still being used for processing will stay in this policy. 
  - **当块类型为Hot时，所有的副本存储为DISK；**
- **Cold** - only for storage with limited compute. The data that is no longer being used, or data that needs to be archived is moved from hot storage to cold storage. 
  - **当块类型为Hot时，所有的副本存储为Archive；**
- **Warm** - partially hot and partially cold. 
  - **当块类型为Hot时，部分的副本存储为DISK，其余副本存储为ARCHIVE；**
- **All_SSD** - 所有副本存储为SSD.
- **One_SSD** - 一个副本SSD，其余副本存储为SSD..
- **Lazy_Persist** - **只适用单个副本的情况**。**第一个副本存储为RAM_DISK，其余副本懒惰式持久化到DISK**
- **Provided** - 在HDFS外存储数据。

当存储类型对应的存储类型的存储没有空间后，有fallback的存储类型。

| **Policy** **ID** | **Policy** **Name** | **Block Placement** **(n replicas)** | **Fallback storages** **for creation** | **Fallback storages** **for replication** |
| :---------------- | :------------------ | :----------------------------------- | :------------------------------------- | :---------------------------------------- |
| 15                | Lazy_Persist        | RAM_DISK: 1, DISK: *n*-1             | DISK                                   | DISK                                      |
| 12                | All_SSD             | SSD: *n*                             | DISK                                   | DISK                                      |
| 10                | One_SSD             | SSD: 1, DISK: *n*-1                  | SSD, DISK                              | SSD, DISK                                 |
| 7                 | Hot (default)       | DISK: *n*                            | <none>                                 | ARCHIVE                                   |
| 5                 | Warm                | DISK: 1, ARCHIVE: *n*-1              | ARCHIVE, DISK                          | ARCHIVE, DISK                             |
| 2                 | Cold                | ARCHIVE: *n*                         | <none>                                 | <none>                                    |
| 1                 | Provided            | PROVIDED: 1, DISK: *n*-1             | PROVIDED, DISK                         | PROVIDED, DISK                            |

## 配置

- **dfs.storage.policy.enabled** 
  - 是否允许存储策略，默认为true。
- **dfs.datanode.data.dir**
  - 如   [DISK]file:///grid/dn/disk0,[SSD]file:///grid/dn/ssd0,[ARCHIVE]file:///grid/dn/archive0,[RAM_DISK]file:///grid/dn/ram0

**默认的datanode存储位置的存储类型为`DISK`。**



## 基于数据移动的存储策略

**在已经存在的file/dir上设置新的存储策略将改变Namespace中的策略，但不会在存储介质上移动块**。

以下两个选项将允许用户基于新的策略集移动块。因此，一旦用户更改/设置文件/目录的新策略，用户还应该执行以下选项之一，以实现所需的数据移动。

**注意，这两个选项不能同时运行。**

### SPS（Storage Policy Satisfier）

如果SPS识别出某个文件需要移动的块，则会将块移动任务调度到datanode。如果在移动中有任何失败，SPS将通过发送新的块移动任务重新尝试。

作为NameNode外的服务，可以单独启动和停止。 [Storage Policy Satisfier(SPS) (HDFS-10285)](https://issues.apache.org/jira/browse/HDFS-10285)

```java
// HdfsAdmin API : 所有的子目录
public void satisfyStoragePolicy(final Path path)
```

- **dfs.storage.policy.satisfier.mode** 
  - `external`或者`none`，默认`none`，不开启SPS；
- **dfs.storage.policy.satisfier.recheck.timeout.millis**
  - 重新检查处理后的块存储移动命令结果的超时时间。
- **dfs.storage.policy.satisfier.self.retry.timeout.millis** 
  - 如果在此配置超时内没有从Datanode报告块移动结果，则进行重试。

### Mover（数据迁移工具）

```shell
hdfs mover [-p <files/dirs> | -f <local file name>]
```

对于违反存储策略的块，它将副本移动到不同的存储类型，以满足存储策略的要求。

只要可能，它总是尝试在同一节点内移动块副本。如果这是不可能的(例如，当一个节点没有目标存储类型时)，它将复制块副本到网络上的另一个节点。

## 命令

```shell
hdfs storagepolicies -listPolicies
hdfs storagepolicies -setStoragePolicy -path <path> -policy <policy>
hdfs storagepolicies -unsetStoragePolicy -path <path>
hdfs storagepolicies -getStoragePolicy -path <path>
# Schedule blocks to move based on file’s/directory’s current storage policy.
hdfs storagepolicies -satisfyStoragePolicy -path <path>

# 允许SPS服务
hdfs dfsadmin -reconfig namenode host:ipc_port start
# 启动SPS服务
hdfs –daemon start sps
```

