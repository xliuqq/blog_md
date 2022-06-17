## GFS

论文



Big Storage的循环问题：

- Performance -> Sharding （性能导致数据分片）
- Sharding -> Faults (数据分片到集群，服务器故障)
- Faults -> Tolerance （容错性)
- Tolerance -> Replication (复制解决容错性）
- Replication -> Inconsistency （复制带来不一致性）
- Consistency -> Low Performance （一致性要求导致低性能）



强一致性



Master MetaData

- file_name -> 
  - array of chunk ids/handles （文件名对应的数据块）非易失性
- chunk handle ->
  - list of chunk servers（chunk所在的服务器）
  - chunk version （版本号）非易失性
    - chunk server可能离线，无法取最大值
  - primary chunk server （主数据块服务器）
  - lease expiration (primary server的租约时间)

恢复

- 磁盘存储：LOG + CHECKPOINT 



Read

- filename + offset  -> chunk handle, chunk servers ；
- client 选择一个chunk server（规则是什么？），读取数据；



Write（append）

- No primary for chunk ? Master will do 

  - Find up to date Replicas
  - Pick prmary, second chunk server
  - Increment chunk version
  - Tells primary, second the newest version
  - Master write newest version to disk

  Master的版本号，低于chunk server？

  - TODO：Master版本号写磁盘和告诉Primary的顺序？



Primary picks offset

all replicas told wo write at offset

IF all 'yes', primary return 'success' to client ELSE 'no' to client



S plit brain