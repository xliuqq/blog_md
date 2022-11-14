# 分布式一致性问题

## CAP理论

Consistency：一致性

Availability：可用性

Partition Tolerant：网络分区容错性



- 单Master

- 协调器（代理)

- 中间件

分布式数据一致性

- 解决的问题？
- Paxos， Raft
  - 分布式进程一定可以对一个数据的取值达成一致；
  - 对一个数据的取值一定可以有一个可取值被提议；
  - 一旦分区进程对一个数据的取值达成一致，所有分区进程最终都可以得到这个取值。



## BASE理论



TODO：[终于有人把“分布式事务”说清楚了！ (qq.com)](https://mp.weixin.qq.com/s/Rowg-rEQZnaSaupJ9Q7kJw)
