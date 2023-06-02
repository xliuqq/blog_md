# Apache Ratis

> Apache Ratis is a highly **customizable Raft protocol** implementation in Java. 





## 概念

TermIndex：

- Term
- Index



**RaftPeer**：Raft 协议中的节点

- **Id**：唯一 Id 表示
- **Address**：
- **AdminAddress**
- **ClientAddress**
- **DataStreamAddress**
- **Priority**
- **StartupRole**

**RaftGroup**

- **RatfGroupId**：标识 Group 的唯一 Id
- **Peers**：`List<RaftPeer>`



**RaftClient**

- **group**：即RaftGroup；

- **clientRpc**：默认不设置，则为`Grpc`，还支持`Netty`和`HadoopRpc`；

- **配置（properties)**：

  - StorageDir：`raft.server.storage.dir`，存储目录
  - serverPort：`raft.grpc.server.port`，

  

**RaftServer**

- **serverId**：RaftPeer的Id：
- **group**：即RaftGroup；
- **配置（properties)**：
  - StorageDir：`raft.server.storage.dir`，存储目录
  - serverPort：`raft.grpc.server.port`，

- **StateMachine**：继承`BaseStateMachine`，最重要的方法
  - load the latest snapshot
  - Apply a committed log entry to the state machine




## 示例

### 全局计数器

```java
public classs AppStateMachine extends BaseStateMachine {
    // 状态机存储（使用 raft.server.storage.dir）
    private final SimpleStateMachineStorage storage = new SimpleStateMachineStorage();
    private final AtomicInteger counter = new AtomicInteger(0);
    
    // Initialize the state machine storage and then load the state.
    @Override
	public void initialize(RaftServer server, RaftGroupId groupId, RaftStorage raftStorage) throws IOException {
        super.initialize(server, groupId, raftStorage);
        storage.init(raftStorage);
        reinitialize();
    }
    
    // Simply load the latest snapshot.
    @Override
    public void reinitialize() throws IOException {
        // 从
        load(storage.getLatestSnapshot());
    }

    // Store the current state as a snapshot file in the {@link #storage}.
    public long takeSnapshot() {
        // 获取 snapshot 文件
        // 写入 state 信息 （包括 Term Index）
    }
    
    // Query the state machine. The request must be read-only.
    @Override
    public CompletableFuture<Message> query(Message request) {
    }
    
    // Apply a committed log entry to the state machine. 
    @Override
    public CompletableFuture<Message> applyTransaction(TransactionContext trx) {
        // TransactionContext 中包含 TermIndex，Message，并且返回 Message 给 Client
    }
}
```

