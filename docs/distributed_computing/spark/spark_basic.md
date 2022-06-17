# Spark基础

## 事件

Spark 2.3 之前，事件处理是单线程的架构，如果作业很大很多 tasks，很可能会导致事件处理出现延迟，进一步导致作业性能出现问题；

- Spark 2.3 引入多线程事件处理架构，事件分为四种类型，每种类型都在一个单线程中顺序处理；



## RPC

> 拆分出来的Spark-rpc用于学习，见 https://gitee.com/import-github/spark-rpc

基于netty自己实现了一套RPC框架，保留了Akka的风格，采用RpcEndpoint模拟Actor, RpcEndpointRef模拟ActorRef, RpcEnv模拟ActorSystem。

- **RpcEnv**，RPC通信的基础，内部包含作为服务端和接收消息组件以及作为客户端发送消息的组件。每个单独的进程会创建一个RpcEnv，目前版本是NettyRpcEnv。 
- **Dispatcher**，用于服务端接收分发消息。内部使用receivers数组缓存接收到的消息，并用线程池轮训处理。 
- **outboxes**，向外发送消息，内部根据不同的endpointref组织到不同的outbox中，即同一接收者的消息会一起存放在相同的outbox中。
