# 高可用



## 主备复制原理

![img](pics/mysql_primary_slave.png)

- MySQL master 将数据变更写入**二进制日志(binary log**，其中记录叫做二进制日志事件binary log events，可以通过 show binlog events 进行查看)；
- MySQL slave 将 master 的 binary log events 拷贝到它的中继日志(replay log)；
- MySQL slave 重放 relay log 中事件，将数据变更反映它自己的数据（**数据延迟**）；



## Binlog

Binlog是二进制格式的日志文件，用来记录数据库的数据更新或者潜在更新(比如DELETE语句执行删除而实际并没有符合条件的数据)，主要用于数据库的主从复制以及增量恢复。

一共有两种类型二进制记录方式：

- **Statement模式**（默认)
  - 每一条会**修改数据的sql**都会被记录在binlog中，如 inserts, updates, deletes；
  - 修改数据的时候使用了某些定的函数或者功能，会造成MySQL的复制出现问题；
- **Row模式:**
  - 每一行的**具体变更事件**都会被记录在binlog中，包含所有的数据；
  - 以每行记录的修改来记录，会产生大量的日志内容。

- Mixed模式是以上两种level的混合使用，默认使用Statement模式，根据具体场景自动切换到Row模式。