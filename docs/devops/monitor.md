# 节点监控

> 监控不能只是各种数据的采集和罗列，不仅仅是弄若干个报表并进一步配置成仪表盘，**而是有一定智能，仿照我们日常的排查问题思路，建立一定规则，自动检查，深度检查，友情提示**。



## Statd

 StatsD 是一个 NodeJs 的 daemon 程序，通过 **UDP 或者 TCP** 方式侦听各种**统计信息**，包括计数器和定时器，并发送**聚合信息**到后端服务。



## Node exporter

Prometheus 生态，用于获取节点的基本运行信息。



## Monit（系统监控和管理工具）

*Monit* 是一个Linux/UNIX系统上开源的**进程、文件、目录和文件系统监控和管理**工具，可自动维护和修复一些错误的情况。

Monit使用C语言编写而成，处理效率非常的高，占用资源非常少（几乎不占用资源），配置参数十分的简单，只使用几个if … then…语句就可以完成监控任务。尤其适用于对某些进程进行守护。例如：在检测到http服务不正常时，自动重启apache或nginx 。

*Monit*是单机的开源工具，M/Monit是分布式的收费企业版。

### 使用文档

https://mmonit.com/monit/documentation/monit.html



## Zabbix（分布式系统监视以及网络监视）

基于WEB界面的提供**分布式系统监视以及网络监视功能的企业级的开源解决方案**，采用C/S架构。

Zabbix 通过 JMX 支持 Java 应用程序监控；通过 IPMI 支持硬件设备监控；通过 SNMP 支持网络设备。

### 组件

**Zabbix Agent** 运行在被监控设备上，负责和 Zabbix Server 通信获取和控制被监控设备，它有主动和被动两种工作模式。

**Zabbix Server** 作为监控核心，可以直接与 Zabbix Agent 连接也可以通过 Zabbix Proxy 进行连接，再由 Zabbix Proxy 连接 Zabbix Agent。后面这种方式用在分布式监控的场景。

Zabbix Server 获取的数据存放到 Zabbix Server 的数据库中，Zabbix UI（Web）可以读取服务器中的数据，通过图表的方式展示出来。

### 使用文档

https://www.zabbix.com/documentation/current/manual



## netdata（实时的资源监控工具）

实时的资源监控工具，它拥有基于 web 的友好界面，通过图表来了解 CPU，RAM，硬盘，网络，Apache， Postfix 等软硬件的资源使用情况。

仅仅支持通过 Web 界面进行实时监控，非并行的可视化（即单节点性能独立展示）。

**可以被集成到已经存在的监控工具中**（Prometheus, Graphite, OpenTSDB, Kafka, Grafana, etc）。

### 集群监控

netdata本身不存在主从服务，在每一个节点上都需要完整部署netdata。

#### netdata.cloud

使用同一账号登陆到netdata.cloud（需要科学上网），各个节点之间就可以轻松通过一个账号控制。

- netdata.cloud只是帮你记录每个节点的信息，而控制面板在获取每个节点的数据的时候，是由前端直接从各个节点的19999端口获取数据的。也就是说，每个节点都必须要打开端口，开启dashboard，允许管理员查看数据。

#### 
