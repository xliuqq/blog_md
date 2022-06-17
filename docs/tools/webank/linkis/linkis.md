# LinkIs

提供scala/java的api，jdbc，和restful 接口。

https://github.com/WeBankFinTech/Linkis-Doc/blob/master/zh_CN/User_Manual/Linkis1.0%E7%94%A8%E6%88%B7%E4%BD%BF%E7%94%A8%E6%96%87%E6%A1%A3.md



## 架构

### PublicEnhencementService（PS）架构设计

公共增强服务，为其他微服务模块提供**统一配置管理**、**上下文服务**、**物理库**、**数据源管理**、**微服务管理**和**历史任务查询**等功能的模块。

https://github.com/WeBankFinTech/Linkis-Doc/blob/master/zh_CN/Architecture_Documents/Public_Enhancement_Services/README.md



### 微服务治理

Gateway、Eureka、Open Feign等三个主要的微服务。用来解决Linkis的服务发现与注册、统一网关、请求转发、服务间通信、负载均衡等问题。

- Linkis Gateway作为Linkis的网关入口，主要承担了请求转发、用户访问认证、WebSocket通信等职责，基于Label的路由转发能力；

- 基于Feign实现了一套自己的底层RPC通信方案，提供SDK集成到有需要的微服务之中。一个微服务既可以作为请求调用方，也可以作为请求接收方。作为请求调用方时，将通过Sender请求目标接收方微服务的Receiver，作为请求接收方时，将提供Receiver用来处理请求接收方Sender发送过来的请求，以便完成同步响应或异步响应。

### 计算治理服务

#### Entrance

- 负责计算任务的接收、调度、转发执行请求、生命周期管理的服务，并且能把计算结果、日志、进度返回给调用方；

#### Orchestrator

- 准备阶段的入口，解析Job、申请Engine和提交执行的能力；
- 编排和计算策略能力，满足多活、主备、事务、重放、限流、异构和混算等多种应用场景的需求。

#### LinkisManager

Linkis的管理大脑，主要由AppManager、ResourceManager、LabelManager和EngineConnPlugin组成。

1. ResourceManager 具备  对 Yarn 和 Linkis EngineManager的资源管理能力，还将提供**基于标签的多级资源分配和回收能力**，让ResourceManager具备跨集群、跨计算资源类型的全资源管理能力；
2. AppManager 将统筹管理所有的 EngineConnManager 和 EngineConn，EngineConn 的申请、复用、创建、切换、销毁等生命周期全交予 AppManager 进行管理；而 LabelManager 将基于多级组合标签，提供跨IDC、跨集群的 EngineConn 和 EngineConnManager 路由和管控能力；
3. EngineConnPlugin 主要用于降低新计算存储的接入成本，真正做到让用户只需要实现一个类，就能接入一个全新的计算存储引擎。

#### EngineConnManager

- 聚焦于支持各类 EngineConn 的启动和销毁

#### EngineConn

提供 EngineConn 和 Executor 两大模块：

- EngineConn 用于连接底层的计算存储引擎，提供一个打通了底层各计算存储引擎的 Session 会话；
- Executor 则基于这个 Session 会话，提供交互式计算、流式计算、离线计算、数据存储的全栈计算能力支持。



## 认证

linkis 没有用户表，`linkis_user`表是dataspherestudio建立的。



LDAP的认证机制

Linkis只有**admin用户和非admin用户**，admin用户的唯一特权，就是支持在Linkis管理台查看所有用户的历史任务。





## 路由（Gateway）

在`linkis_ps_instance_info`中定义`linkis-cg-entrance`、`linkis-cg-entrance`等服务的实例

- 启动时注册，

