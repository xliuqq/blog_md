# Jaeger

> CNCF 基金会开源的分布式追踪系统。
>
> open source, end-to-end distributed tracing
>
> Monitor and troubleshoot transactions in complex distributed systems



## 架构

![img](pics\jaeger_arch.jpg)

**jaeger-client**：jaeger 的客户端，实现了opentracing协议；

**jaeger-agent**：jaeger client的一个代理程序，client将收集到的调用链数据发给agent，然后由agent发给collector；

**jaeger-collector**：负责接收jaeger client或者jaeger agent上报上来的调用链数据，然后做一些校验，比如时间范围是否合法等，最终会经过内部的处理存储到后端存储；

**jaeger-query**：专门负责调用链查询的一个服务，有自己独立的UI；

**jaeger-ingester**：中文名称“摄食者”，可用从kafka读取数据然后写到jaeger的后端存储，比如**Cassandra和Elasticsearch**；

**spark-job**：基于spark的运算任务，可以计算服务的依赖关系，调用次数等；



## API

使用 OpenTelemetry
