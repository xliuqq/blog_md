# Prometheus

服务监控系统和时间序列数据库。

> 一个监控系统，它不仅仅包含了时间序列数据库，还有全套的抓取、检索、绘图（集成Grafana）、报警的功能。

## 架构

<img src="pics/prometheus_arch.png" alt="img" style="zoom: 50%;" />

### 基本原理

Prometheus的基本原理是通过**HTTP协议周期性抓取被监控组件的状态**。



### 服务过程

- Prometheus Daemon负责定时去目标上抓取metrics(指标)数据，每个抓取目标需要暴露一个http服务的接口给它定时抓取。Prometheus支持通过**配置文件、文本文件、Zookeeper、Consul、DNS SRV Lookup等方式**指定抓取目标。Prometheus采用PULL的方式进行监控，即服务器可以直接**通过目标PULL数据或者间接地通过中间网关来Push数据**。
- Prometheus在**本地存储抓取的所有数据**，并通过一定规则进行清理和整理数据，并把得到的结果**存储到新的时间序列**中。
- Prometheus支持很多方式的图表可视化，例如**Grafana**、自带的Promdash以及自身提供的模版引擎等等。Prometheus还提供HTTP API的查询方式，自定义所需要的输出。
- PushGateway**支持Client主动推送metrics到PushGateway**，而Prometheus只是定时去Gateway上抓取数据。
- Alertmanager是独立于Prometheus的一个组件，可以支持Prometheus的查询语句，提供十分灵活的**报警**方式。

### 三大套件

- Server 主要负责数据采集和存储，提供PromQL查询语言的支持。
- Alertmanager 警告管理器，用来进行报警。
- Push Gateway 支持临时性Job主动推送指标的中间网关。



## 服务发现

`prometheus`是通过server端配置里的`targets`来获取监控对象的，targets 依赖discovery模块进行获取源。

### 静态配置

修改配置文件`prometheus.yml`，然后重启 Prometheus

```yaml
  - job_name: 'nodes'
    static_configs:    #静态配置
    - targets:
       - 172.16.78.12:9100  #node_exporter启动端口
       - 172.16.78.18:9100
       - 172.16.78.13:9100
```

### 动态发现

基于服务注册中心自动发现、检测、分类可被监控的各target，目前支持:

- 文件（实时）、[HTTP](https://prometheus.io/docs/prometheus/latest/http_sd/)（轮询）；
- consul，eureka，zookeeper；
- DNS，K8s；

#### K8s(TODO)



## 集群监控

**联邦机制**

- **主节点配置文件可以配置从节点的地址池**，**主节点只要定时向从节点拉取数据即可**，主节点的作用就是存贮实时数据，并提供给grafana 使用。而从节点作用就是分别从不同的各个采集端中抽取数据，可以实现分机器或者分角色。

Prometheus社区提供了远程读写两个接口，使用一些社区的时序数据库方案，来扩展prometheus。

[实现对比](https://zhuanlan.zhihu.com/p/135368139)

### [Thanos](https://github.com/thanos-io/thanos)



### [Cortex]((https://github.com/cortexproject/cortex))

> A horizontally scalable, highly available, multi-tenant, long term Prometheus



## 部署

yaml 配置

```yaml
global:
  scrape_interval:     15s # By default, scrape targets every 15 seconds.

  # Attach these labels to any time series or alerts when communicating with
  # external systems (federation, remote storage, Alertmanager).
  external_labels:
    monitor: 'codelab-monitor'

# A scrape configuration containing exactly one endpoint to scrape:
# Here it's Prometheus itself.
scrape_configs:
  # The job name is added as a label `job=<job_name>` to any timeseries scraped from this config.
  - job_name: 'prometheus'
    # Override the global default and scrape targets from this job every 5 seconds.
    scrape_interval: 5s
    static_configs:
      - targets: ['localhost:9090']
```



```shell
docker pull prom/node-exporter
docker run -d -p 9100:9100 \
  -v "/proc:/host/proc:ro" \
  -v "/sys:/host/sys:ro" \
  -v "/:/rootfs:ro" \
  prom/node-exporter

docker pull prom/prometheus
docker run  -d \
  -p 9090:9090 \
  -v /home/workspace/prometheus/prometheus.yml:/etc/prometheus/prometheus.yml  \
  prom/prometheus
```



## Gateway

客户端使用push的方式上报监控数据到`pushgateway`，`prometheus`会定期从`pushgateway`拉取数据。

场景：

- Prometheus 采用 pull 模式，可能由于不在一个子网或者防火墙原因，Prometheus 无法拉取数据；
- 没有端口可以暴露metrics，或者Job启动周期太短；

劣势：

- gateway会成为瓶颈，单点故障；
- 丢失自动的实例监控监控；
- 必须手动删除任何过时的指标，或者自己自动执行这个生命周期同步。



### 配置

`/etc/prometheus/prometheus.yml`

```yaml
  - job_name: "pushgateway"	
    honor_labels: true			#检测是否有重复的标签
    file_sd_configs:
    - files:
      - targets/pushgateway/*.json
      refresh_interval: 5m				#服务发现间隔
```

从 pushgateway 抓取的指标的`instance` label 为 pushgateway 的主机和端口配置。



## 自实现

https://github.com/prometheus/client_java#exporting

