# [Etcd](https://github.com/etcd-io/etcd)

> Distributed reliable key-value store for the most critical data of a distributed system.
>
> 解决分布式中数据一致性的问题，数据分为控制数据和应用数据。
>
> etcd处理的数据默认为控制数据，对于应用数据只推荐处理数据量小但更新访问频繁的情况。

## 场景

特点：

- *Simple*: well-defined, user-facing API (gRPC)
- *Secure*: automatic TLS with optional client cert authentication
- *Fast*: Benchmarked at 1000s of writes/s per instance
- *Reliable*: properly distributed using Raft

使用场景：

- 服务发现
- 消息发布和订阅
- 负载均衡
- 分布式通知和协调
- 分布式锁与竞选
- 分布式队列
- 集群监控
