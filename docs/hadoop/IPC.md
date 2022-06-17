[toc]

# IPC

IPC：进程间通信，Inter-Process Communication。

RPC：远程过程调用，Remote Procedure Call，实现 IPC。



## Engine

Hadoop有两个`RPCEngine`实现：

- `WritableRpcEngine`：将请求序列化发送到Server端，通过反射查找匹配的方法，执行并返回结果；
- `ProtobufRpcEngine`：基于 protobuf 2版本；



**如何决定是WritableEngine还是ProtobufEngine**

- 通过`RPC.setProtocolEngine()`注册某个接口的Engine类型，如果没有注册，`RPC.getProtocolEngine`默认逻辑会走`WritableRpcEngine`；
- 







### Writable



### Protobuf



 版本号



## Server（服务端）

```java
RPC.Builder builder = new RPC.Builder(conf);
builder.setProtocol(MPDProtocol.class);
builder.setInstance(this);
builder.setBindAddress("0.0.0.0");
builder.setPort(0);
Server server = builder.build();
```



## Proxy(客户端)

```java
// addr是重点
RPC.getProxy(MPDProtocol.class, MPDProtocol.versionID, addr, conf);
```

YarnClient 获取 ApplicationReport # getRpcPort 获取 Application 的 Rpc端口，Client可以通过这个端口连接AM RPC；

AM 启动 Container时，通过将AM的Rpc端口设置Container的环境变量；



## 版本号

Client端依赖的Protocol的版本号，跟Server端的Protocol可能不一样。

- 接口中定义`long versionID = 234234L`的字段

- 或接口实现`VersionedProtocol`的接口