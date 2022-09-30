# 短路读

Short-Cut Read：client和Block在同一节点时，跳过DataNode，直接读取本地文件；



## 配置

short-cut 读取使用UNIX domain socket，client和DataNode通过**/dev/shm的共享内存段**交换信息。

前提：**开启 libhadoop.so**，见[本地库](native_libraries.md)；

```xml
<!--示例配置-->
<configuration>
  <property>
    <name>dfs.client.read.shortcircuit</name>
    <value>true</value>
  </property>
  <property>
    <name>dfs.domain.socket.path</name>
    <value>/var/lib/hadoop-hdfs/dn_socket</value>
  </property>
</configuration>
```



## 实现

共享内存可以通过POSIX提供的**mmap接口**实现将文件映射到内存，并且映射数据是实时同步。



TODO：具体的流程分析

https://blog.csdn.net/b6ecl1k7bs8o/article/details/90981844

