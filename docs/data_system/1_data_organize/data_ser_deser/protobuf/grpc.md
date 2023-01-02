# Grpc

## 通信技术的演化

**传统的RPC**：如 COBRA 和 JMI 等；

- 构建在TCP之上，妨碍互操作性且有大量的规范限制；

**SOAP**：简单对象访问协议

- **面向服务**的架构的标准通信技术，通过XML交换数据，支持任务底层通信协议，最常见为HTTP；

**REST**：描述性状态迁移，**面向资源**的架构，资源的创建、读取、更新和删除；

- 基于文本的低效消息协议，如 JSON/XML（HTTP1.x）；
- REST架构风格难以强制实施，只有遵循实践，但不能够强制要求；
- 应用程序缺乏强类型接口，OpenAPI/Swagger都是事后补救措施；

## 场景

**优势**：

- 高效的进程间通信，基于protobuf二进制；
- 简单且定义良好的服务接口和服务；
- 强类型，多语言；
- 双工流，客户端和服务端都支持流；
- 内置商业化特性，如认证、加密、弹性、压缩、负载均衡、服务发现等；
- 云原生系统集成，支持很好；

**劣势**：

- grpc不太适合面向外部的服务，grpc网关是解决方案；
- 巨大的服务定义变更是复杂的开发流程，不要引入破坏性的变更（protobuf的兼容性）；
- 浏览器和移动程序对gRPC的支持初级阶段不太成熟（2022年）；

## 原理

> 基于HTTP/2 + Protobuf，支持互联网规模的进程间通信技术。

**一个Channel对应底层一个TCP连接，只是基于HTTP/2实现多路 复用。**

一个TCP连接可以同时进行多个RPC，一次RPC为HTTP/2中的一个Stream。

- 将**服务注册到gRPC内部的服务注册中心**上，根据服务名和方法名直接调用注册的服务实例，不需要反射的方式进行调用。

### Stream vs Repeated

**长时间运行的临时数据(如通知或日志消息)使用流特性。**

取决于以下因素:

- 数据集的总体大小。

- 在客户端或服务器端创建数据集所花费的时间。

- 数据集的使用者是否可以在第一项可用时立即对其进行操作，或者需要完整的数据集来做任何有用的事情。

任何在大小上受到限制的数据集，并且可以在短时间内(比如在一秒钟内)完整生成的数据集，您应该在常规Protobuf消息中使用重复字段。

数据集中的消息对象可能非常大时，最好使用流请求或响应来传输它们。



### TODO（Zero copy）



## 模式

### unary

### client streaming

### server streaming

### bidirectional streaming



## 拦截器



## 线程模型（Java）

### 服务端线程模型

- 服务端监听和客户端接入线程（HTTP/2 Acceptor）
- 网络I/O读写线程
- 服务接口调用线程

![grpc服务端线程模型交互](./pics/grpc_server_thread_model)

其中：HTTP/2服务端创建、HTTP/2请求消息的接入和响应发送由Netty负责；gPRC消息的序列化和反序列化接口调用由gRPC的SerializingExecutor的线程池负责。

- NettyServer实例创建：由NettyServerBuilder的buildTransportServers方法构建。
- 在 gRPC 中，默认采用共享池模式创建 NioEventLoopGroup，所有的 gRPC 服务端实例，都统一从 SharedResourceHolder 分配 NioEventLoopGroup 资源，实现 NioEventLoopGroup 的共享。



工作流程总结如下：

- 从主线程池（bossGroup）中随机选择一个 Reactor 线程作为 Acceptor 线程，用于绑定监听端口，接收客户端连接；
- Acceptor 线程接收客户端连接请求之后创建新的 SocketChannel，将其注册到主线程池（bossGroup）的其它 Reactor 线程上，由其负责接入认证、握手等操作；
- 步骤 2 完成之后，应用层的链路正式建立，将 SocketChannel 从主线程池的 Reactor 线程的多路复用器上摘除，重新注册到 Sub 线程池（workerGroup）的线程上，用于处理 I/O 的读写操作。



Netty 的 NIO 线程实现是 **NioEventLoop**，它的职责如下：

- 作为服务端 Acceptor 线程，负责处理客户端的请求接入；
- 作为客户端 Connector 线程？？，负责注册监听连接操作位，用于判断异步连接结果；
- 作为 I/O 线程，监听网络读操作位，负责从 SocketChannel 中读取报文；
- 作为 I/O 线程，负责向 SocketChannel 写入报文发送给对方，如果发生写半包，会自动注册监听写事件，用于后续继续发送半包数据，直到数据全部发送完成；
- 作为定时任务线程，可以执行定时任务，例如链路空闲检测和发送心跳消息等；
- 作为线程执行器可以执行普通的任务 Task（Runnable）。

**NioEventLoop同时支持I/O操作和Runnable执行的原因如下：避免锁竞争，例如心跳检测，往往需要周期性的执行**。

gRPC 服务端调度线程为 **SerializingExecutor**，它实现了 Executor 和 Runnable 接口，通过外部传入的 Executor 对象，调度和处理 Runnable，同时内部又维护了一个任务队列ConcurrentLinkedQueue，通过 **run 方法循环处理队列中存放的 Runnable 对象**。

当服务端接收到客户端HTTP/2请求消息时，由Netty的NioEventLoop线程切换到gRPC的SerializingExecutor（每个Stream创建都会创建该实例），进行消息的反序列化、以及服务接口的调用，**SerializeingExecutor在默认配置下会用默认的共享线程池去执行**。

gRPC 的线程模型遵循 Netty 的线程分工原则，即：

- 协议层消息的接收和编解码由 Netty 的 I/O(NioEventLoop) 线程负责；
- gRPC service线程（应用线程），防止由于应用处理耗时而阻塞 Netty 的 I/O 线程。
  - 将gRPC请求消息反序列化为接口的请求参数对象；
  - 将接口响应为对象序列为PB码流；
  - gRPC服务端接口实现类调用。



### 客户端线程模型

- 客户端连接线程（HTTP/2 Connector）

- 网络I/O读写线程

- 接口调用线程

- 响应回调通知线程


客户端线程模型工作原理如下图所示（同步阻塞调用为例）：

![图片](./pics/grpc_client_threads.jpeg)

- 应用线程，负责调用 gRPC 服务端并获取响应，其中请求消息的序列化由该线程负责。
- 客户端负载均衡以及 Netty Client 创建，由 grpc-default-executor 线程池负责。
- HTTP/2 客户端链路创建、网络 I/O 数据的读写，由 Netty NioEventLoop 线程负责。
- 响应消息的反序列化由 SerializingExecutor 负责，与服务端不同的是，客户端使用的是 ThreadlessExecutor，并非 JDK 线程池。
- SerializingExecutor 通过调用 responseFuture 的 set(value)，唤醒阻塞的应用线程，完成一次 RPC 调用。



**gRPC 采用的是网络 I/O 线程和业务调用线程分离的策略**，缺点就是在**一次 RPC 调用过程中，做了多次 I/O 线程到应用线程之间的切换，频繁切换会导致性能下降**。



## 实践

### Alluxio Grpc

gRPC高效传输数据的一些技巧：

- **使用多个channel实现最大吞吐量**：虽然大多数短RPC调用都受益于多路复用，但数据传输需要利用所有网络带宽，而这是单个连接所不具备的。根据gRPC团队的建议，我们转而使用多个**channel**来最大化我们的数据传输吞吐量。
- **使用更大的块大小**：发送每条消息都会产生一些开销，例如消息头和上下文切换。对于相同数量的数据，每个块越小，管道必须处理的块越多，这反过来会引入更多的开销。我们最终使用相对较大的块大小来最小化这种影响。
- **不要丢弃Netty**：即使所有数据都是通过gRPC发送的。鉴于gRPC没有公开能够处理Netty ByteBuf的公共接口，我们在gRPC服务端点的早期实现中切换到使用Java NIO ByteBuffer。即使我们使用了堆外缓冲区，也还是引入了一些显著瓶颈。这里学到的教训是不要因为gRPC没有公开Netty缓冲区的接口而摆脱Netty。**Netty提供了一些非常有效的API来管理缓冲区**，这将使gRPC管道之外的东西依然保持迅速。
- **实现零复制**：我们观察到切换到gRPC的另一个开销是过多的缓冲区复制。这是因为默认情况下，gRPC使用protobuf进行消息序列化，这会引入额外的复制操作。目前还没有能在**无需经过protobuf序列化或抛弃生成好的RPC绑定的情况下存取gRPC原始缓冲区的官方版的API**。我们最终使用了gRPC用户组的一些想法，实现了一种零复制的替代方案。
  - 1.39.0+：[api, core: support zero copy into protobuf by voidzcy · Pull Request #8102 · grpc/grpc-java · GitHub](https://github.com/grpc/grpc-java/pull/8102)
- **使用手动背压控制**(back pressure control)：gRPC Java SDK中的背压控制是通过流观察器(stream observer)的isReady标志和onReady事件提供的。一旦缓冲区中的数据小于32KB，就会触发onReady事件。这是一个相对较小的缓冲区大小，并且可能会导致一些争用，因为发送方一直在等待从缓冲区中删除数据，特别是当我们增加了块大小后，这种争用更加明显。不幸的是，截至目前，我们无法调整缓冲区大小。我们最终实现了手动背压控制，。
- 

Alluxio的Grpc传输文件比自实现的Grpc性能要好，服务端发送完数据后，客户端需要一定时间进行读取，目前不清楚这部分性能消耗点！