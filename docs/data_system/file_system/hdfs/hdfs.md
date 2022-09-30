[toc]

# Hadoop Distributed File System

以 2.10.0版本/3.2.1版本 进行介绍



[TODO: 简介](pdf/hdfs_introduction.pptx)

[HDFS HA](HA.md)

[ShortCutRead](short_cut_read.md)

[NativeLibraries](native_libraries.md)

[权限控制](permissions.md)

[文件/目录缓存](centralized_cache.md)

[目录配额](quotas.md)

[存储策略](./storage_policy.md)

[中心化缓存管理](./centralized_cache.md)

[内存存储](./memory_storage.md)



FSImage底层采用Protobuf进行序列化



TODO：



FSDataInputStream中读取特定位置的字节数方法，两者区别在哪？

1. seek(positionRead) + readFully(bytes, offset, len)
2. readFully(positionRead, bytes, offset, len)



Block的校验和是由Client做，还是有DataNode做？



## 心跳机制

