[toc]

# 元数据恢复原理解析

DataNodes存储的块和位置信息，不存储在NameNode元数据，而是NameNode启动时，DataNode汇报其存储在块和位置信息。



## FSImage



## EditLog

FSEditLogOp 基类，不同的操作定义不同类，实现序列化/反序列化方法。

Writer类写Op实例：

- Op名；
- op序列化后的长度；（可选）
- op序列化；

- 校验和；



优化：

- 日志同步写：ThreadLocal的Cache，保存Op实例，避免重复创建和GC开销。

- 