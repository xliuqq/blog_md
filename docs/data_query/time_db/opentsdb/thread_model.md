# 线程模型

在OpenTSDB中，对HTTP请求和HBase响应是**异步处理**的，这部分主要是借助了Netty的框架。异步处理使得线程资源的利用更加高效，一定程度上能够提升读写的并发度，带来吞吐量的提升。目前，HBase也已经支持了异步RPC框架。

## HTTP请求处理线程池

OpenTSDB启动时，会创建一个Netty服务端（默认端口为4242），以接收OpenTSDB客户端发送过来的HTTP请求。Netty的服务端实现包括了一个Boss线程和多个业务处理线程（默认线程数为CPU核数的两倍）。

![HTTP-Request](pics/opentsdb_http_request.png)

- 每个socket绑定I/O线程池中的某个线程。

## HBase响应处理线程池

OpenTSDB发送请求到HBase后，需要处理HBase返回的相应消息，这是通过OpenTSDB中的一个Netty客户端实现的。

Netty的客户端实现包括了一个Boss线程和多个业务处理线程（默认线程数为CPU核数*2）。

![HTTP-Response](pics/opentsdb_http_response.png)

- 每个HBase与OpenTSDB之间的Socket连接，在OpenTSDB向HBase发起请求的时候就已经建立了。 也会把该Socket绑定到AsyncHBase I/O Threads线程池的一个线程中。
- AsyncHBase I/O Thread读取Socket中的HBase响应消息。

 **说明**：上述步骤是在Netty的框架中完成的。

- AsyncHBase I/O Thread 处理HBase的响应消息。而后，发送HTTP响应消息到OpenTSDB应用端。



