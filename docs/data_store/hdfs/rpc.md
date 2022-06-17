# HDFS中RPC

## DataNode

### 心跳

### BlockReport

默认是每6小时blockreport一次，**存在的意义呢**？集群启动的时候会report一次，后续对文件的新增、删除相关操作首先是在namenode操作的。所以namenode是有最新block信息的。

TODO：分析下，这么长的BlockReport。。