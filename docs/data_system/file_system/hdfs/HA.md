

[toc]

# HA

NameNode：分为**Active**和**StandBy**两种状态



NameNodes间元数据一致

- **NFS**：实现多个NameNodes的EditLog共享
- **QJM**：大多数决策，接收元数据修改日志，StandBy读取修改日志



快速的故障切换

- DataNode向所有NameNodes发送块位置信息和心跳



隔离，防止脑裂，保证在任何时候只有一个主NN

- 通知ssh或脚本命令，强行终止前任NameNode



自动故障切换

- 需要配置Zookeeper集群
- ZKFailoverController进程监控NameNode状态

