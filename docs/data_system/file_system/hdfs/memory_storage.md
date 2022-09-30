[toc]

# HDFS的内存存储

HDFS支持向数据节点管理的堆外内存写入。数据节点会将**内存中的数据异步刷新到磁盘**，对性能敏感的IO路径中移除昂贵的磁盘IO和校验和计算。

DFS为惰性持久化写提供了最好的持久性保证。**在副本持久化到磁盘之前重新启动节点，可能会发生罕见的数据丢失。**

应用程序可以**选择使用惰性持久化写，以降低一些持久性保证来获取减少延迟**。

> Memory storage is for applications that **run within the cluster and collocated with HDFS Data Nodes**. 
>
> We have observed that the **latency overhead from network replication negates the benefits of writing to memory**.

![Lazy Persist Writes](pics/hdfs_LazyPersistWrites.png)

## 配置

hdfs-site.xml

```xml
    <property>
    	<name>dfs.datanode.max.locked.memory</name>
        <value>34359738368</value>
    </property>
	<property>
    	<name>dfs.datanode.data.dir</name>
        <value>/grid/0,/grid/1,/grid/2,[RAM_DISK]/mnt/dn-tmpfs</value>
    </property>
	<proerty>
        <name>dfs.storage.policy.enabled</name>
        <value>true</value>
	</proerty>
```

Mount Ram Disk

```shell
$ sudo mount -t tmpfs -o size=32g tmpfs /mnt/dn-tmpfs/
```

建议在/etc/fstab中创建一个条目，以便在**节点重启时自动重新创建RAM磁盘**。



## 使用

```shell
$ hdfs storagepolicies -setStoragePolicy -path <path> -policy LAZY_PERSIST
```

或

```java
// FileSystem API
fs.setStoragePolicy(path, "LAZY_PERSIST");
```

或

```java
// FileSystem#create API
FSDataOutputStream fos = fs.create(
                            path,
                            FsPermission.getFileDefault(),
                            EnumSet.of(CreateFlag.CREATE, CreateFlag.LAZY_PERSIST),
                            bufferLength,
                            replicationFactor,
                            blockSize,
                            null);
```

