# Kubernetes

> 在大规模集群中的各种任务之间运行，实际上存在各种各样的关系。这些关系的处理才是作业编排和管理系统最困难的地方。
>
> 过去很多集群管理项目（Yarn/Mesos）擅长将一个容器按照某种规则放置在某个最佳的节点上运行，称为**调度**；
>
> Kubernetes擅长按用户的意愿和整个系统的规则，自动化处理好容器间的各种关系，称为**编排**；

以**统一的方式抽象底层基础设施能力**（如计算、网络、存储），**定义任务编排的各种关系**（如亲密、访问、代理关系），将抽象以**声明式API**的方式对外暴露，从而允许平台构建者基于这些抽象进一步构建自己的PAAS平台。



## 组件

![Kubernetes 的组件](pics/components-of-kubernetes.svg)

### 控制平面组件（Control Plane Components） 

控制平面的组件对集群做出**全局决策**（比如调度），以及**检测和响应集群事件**（例如，当不满足部署的 `replicas` 字段时，启动新的 pod。

控制平面组件可以在集群中的任何节点上运行，**一般而言，会部署在一个节点，同时该节点不运行用户容器**。

#### [kube-apiserver](https://kubernetes.io/zh/docs/reference/command-line-tools-reference/kube-apiserver/)

> 从 etcd 读取（`ListWatch`）全量数据，并缓存在内存中；**无状态服务**，可水平扩展；

该组件公开了 Kubernetes API，支持水平伸缩。



#### kube-controller-manager

运行控制器进程的控制平面组件。

从逻辑上讲，每个[控制器](https://kubernetes.io/zh/docs/concepts/architecture/controller/)都是一个单独的进程， 但是为了降低复杂性，它们都被编译到同一个可执行文件，并在一个进程中运行。

- 节点控制器（Node Controller）: 负责在节点出现故障时进行通知和响应
- 任务控制器（Job controller）: 监测代表一次性任务的 Job 对象，然后创建 Pods 来运行这些任务直至完成
- 端点控制器（Endpoints Controller）: 填充端点(Endpoints)对象(即加入 Service 与 Pod)
- 服务帐户和令牌控制器（Service Account & Token Controllers）: 为新的命名空间创建默认帐户和 API 访问令牌



#### kube-scheduler

负责监视新创建的、未指定运行节点（node）的 Pods，选择节点让 Pod 在上面运行。



#### ClusterState Store（ETCD 数据库）

保存 Kubernetes 所有集群数据的后台数据库。



### Node 组件 

节点组件在每个节点上运行，**维护运行的 Pod** 并提供 Kubernetes 运行环境。

kubelet 接收一组通过各类机制提供给它的 PodSpecs，确保这些 PodSpecs 中描述的容器处于运行状态且健康。

kubelet 不会管理不是由 Kubernetes 创建的容器。

#### kubelet

每个节点（node）上运行的代理。 它保证容器（containers）都 运行在 Pod 中。

- 宿主机进程：注册到`/etc/systemd/system/kubelet.service`

- `syncFrequency`：默认`1min`，表示在运行中的容器与其配置之间执行同步操作的最长时间间隔；

  

#### kube proxy 

每个节点上运行的网络代理， **实现 Kubernetes 服务（Service） 概念**的一部分。

**维护节点上的网络规则**，允许从集群内部或外部的网络会话与 Pod 进行网络通信。



#### ContainerRuntime

容器运行环境是负责**运行容器的软件**。[Kubernetes CRI (容器运行环境接口)](https://github.com/kubernetes/community/blob/master/contributors/devel/sig-node/container-runtime-interface.md) 的任何实现，包括 Docker、containerd、CRI-O；



### 插件

插件使用 Kubernetes 资源（DaemonSet、 Deployment等）实现集群功能。

插件列表完整列表见[安装扩展（Addons） | Kubernetes](https://kubernetes.io/zh/docs/concepts/cluster-administration/addons/)。

- [Calico](https://docs.projectcalico.org/v3.11/getting-started/kubernetes/installation/calico) 是一个安全的 L3 网络和网络策略驱动；
- [CoreDNS](https://coredns.io/) 是一种灵活的，可扩展的 DNS 服务器，可以 [安装](https://github.com/coredns/deployment/tree/master/kubernetes)为集群内的 Pod 提供 DNS 服务。

#### DNS 

集群 DNS 是一个 DNS 服务器，和环境中的其他 DNS 服务器一起工作，它为 Kubernetes 服务提供 DNS 记录。

**Kubernetes 启动的容器自动将此 DNS 服务器包含在其 DNS 搜索列表中。**

#### Web 界面（仪表盘）

[Dashboard](https://kubernetes.io/zh/docs/tasks/access-application-cluster/web-ui-dashboard/) 是 Kubernetes 集群的通用的、基于 Web 的用户界面。 它使用户可以管理集群中运行的应用程序以及集群本身并进行故障排除。

#### 容器资源监控

[容器资源监控](https://kubernetes.io/zh-cn/docs/tasks/debug/debug-cluster/resource-usage-monitoring/) 将关于容器的一些常见的时间序列度量值保存到一个集中的数据库中，并提供用于浏览这些数据的界面。

#### 集群层面日志

[集群层面日志](https://kubernetes.io/zh/docs/concepts/cluster-administration/logging/) 机制负责将容器的日志数据保存到一个集中的日志存储中，该存储能够提供搜索和浏览接口。



## 架构

### 控制器模式

K8s 作为一个“容器编排”平台，其核心的功能是编排，Pod 作为 K8s 调度的最小单位，具备很多属性和字段，K8s 的编排正是**通过一个个控制器根据被控制对象的属性和字段**来实现。

K8s 里面是 kube-controller-manager 这个组件在做这件事，可以看下 K8s 项目的 pkg/controller 目录，里面包含了所有控制器，都以独有的方式负责某种编排功能，但是它们都遵循一个通用编排模式，即：调谐循环（Reconcile loop），其伪代码逻辑为：

```go
for {
    actualState := GetResourceActualState(rsvc)
    expectState := GetResourceExpectState(rsvc)
    if actualState == expectState {
    // do nothing
    } else {
        Reconcile(rsvc)
    }
}
```

就是一个无限循环（实际是事件驱动+定时同步来实现，不是无脑循环）不断地对比期望状态和实际状态：

- 如果有出入则进行 Reconcile（调谐）逻辑将实际状态调整为期望状态。期望状态就是我们的对象定义（通常是 YAML 文件），实际状态是集群里面当前的运行状态（通常来自于 K8s 集群内外相关资源的状态汇总）；
- 控制器的编排逻辑主要是第三步做的，这个操作被称为调谐（Reconcile），整个控制器调谐的过程称为“Reconcile Loop”，调谐的最终结果一般是对被控制对象的某种写操作，比如增/删/改 Pod。



### 日志架构

 在集群中，**日志应该具有独立的存储和生命周期**，与节点、Pod 或容器的生命周期相独立。 这个概念叫 ***集群级的日志*** 。



#### 节点级组件日志

![节点级别的日志记录](pics/logging-node-level.png)



容器化应用写入 **`stdout`** 和 **`stderr`** 的任何数据，都会被**容器引擎捕获并被重定向到某个位置**。

- docker作为k8s容器运行时的情况下，容器日志的落盘由docker来完成
  - 默认`json-file`格式，且无限制大小，需要修改`/etc/docker/daemon.json`；
  - Pod的日志位置：`/var/log/pods/*/*.log`，是`kubelet`建立的软链；

- 使用某 *CRI 容器运行时* 时，**kubelet 要负责对日志进行轮换**，并管理日志目录的结构：

  - kubelet 将此信息发送给 CRI 容器运行时，后者将容器日志写入到指定的位置；

  - Pod的日志位置：`/var/log/pods/*/*.log`；

  - 在 kubelet 配置文件参数 `containerLogMaxSize` 和 `containerLogMaxFiles` 用来配置每个日志文件的最大长度和每个容器可以生成的日志文件个数上限；


节点级日志记录中，需要重点考虑**实现日志的轮转**，以此来保证日志不会消耗节点上全部可用空间。

- 需要配合`logrotare`来进行 , 日志超过最大限制 , 自动进行rotate操作；
- `kubectl logs` 仅可查询到最新的日志内容，滚动的日志无法查看；



系统组件有两种类型：**在容器中运行**的和**不在容器中运行**的。例如：

- 在容器中运行的 **kube-scheduler** 和 **kube-proxy**；
  - 将日志写到 `/var/log` 目录，绕过了默认的日志机制，使用 [klog](https://github.com/kubernetes/community/blob/master/contributors/devel/sig-instrumentation/logging.md) 日志库；
- 不在容器中运行的 **kubelet** 和**容器运行时**；
  - 使用 **systemd 机制**的服务器上，kubelet 和容器容器运行时将日志写入到 [**journald**](../../linux/journald.md) 中。 
  - 如果没有 systemd，它们将日志写入到 `/var/log` 目录下的 `.log` 文件中
- `/var/log` 目录中的系统组件日志，被工具 `logrotate` 执行每日轮转，或者日志大小超过 100MB 时触发轮转；



## 插件机制

### CRD

> Custom Resource Definition

### CRI

> Container Runtime Interface

containerd

CRI-O

Docker

### CSI

> Container Storage Interface

[Rook](https://www.rook.io/)：基于Ceph的Kubernetes存储插件，加入水平扩展、迁移、灾难备份、监控等大量的企业级功能。

具体见：[CSI 详细介绍](./k8s_csi.md)

### CNI

> Container Network Interface

Weave

Calico

Flannel

Canal

Romana





## Webhook

k8s apiserver 在处理每一个操作资源对象的请求时，在经过认证（是否为合法用户）/ 鉴权（用户是否拥有权限）后，并不会直接根据端点资源类型和 rest 动作直接执行操作，在这中间，请求会被一系列准入控制器插件（Admission Controller）进行拦截，校验其是否合乎要求，亦或是对特定资源进行配置。

### 准入机制

- 如果所有的 webhooks 批准请求，准入控制链继续流转；
- 如果有任意一个 webhooks 阻止请求，那么准入控制请求终止，并返回第一个 webhook 阻止的原因。其中，多个 webhooks 阻止也只会返回第一个 webhook 阻止的原因；
- 如果在调用 webhook 过程中发生错误，那么请求会被终止或者忽略 webhook

### 准入控制器类型

1. **变更** (**Mutating**)：解析请求并在请求向下发送之前对请求进行**更改**；第一阶段
2. **验证** (**Validating**)：解析请求并根据特定数据进行验证；第二阶段

### 动态准入控制

以webhook 的形式去编写自定义的准入控制器，对特定资源的请求进行定制修改和验证，典型的场景有通过 mutating 类型 webhook 注入 side-car 到 pod。

以 kubebuilder 脚手架为例，实现`Handler`接口`Handle(context.Context, Request) Response`

```go
import (
	"github.com/go-logr/logr"
	"sigs.k8s.io/controller-runtime/pkg/client"
	"sigs.k8s.io/controller-runtime/pkg/manager"
	"sigs.k8s.io/controller-runtime/pkg/webhook"
)

// 定义webhook的path和处理的资源，针对create和update的pods

// +kubebuilder:webhook:path=/mutate-fluid-io-v1alpha1-schedulepod,mutating=true,failurePolicy=fail,sideEffects=None,admissionReviewVersions=v1;v1beta1,groups="",resources=pods,verbs=create;update,versions=v1,name=schedulepod.fluid.io
var (
	// HandlerMap contains admission webhook handlers
	HandlerMap = map[string]common.AdmissionHandler{
		common.WebhookSchedulePodPath: &CreateUpdatePodForSchedulingHandler{},
	}
)
	
func Register(mgr manager.Manager, client client.Client, log logr.Logger) {
	server := mgr.GetWebhookServer()
	for path, handler := range HandlerMap {
		handler.Setup(client)
        // 注册 handler
		server.Register(path, &webhook.Admission{Handler: handler})
		log.Info("Registered webhook handler", "path", path)
	}
}
```

## Kubelet

### Static Pod

Kubernetes中有一种特殊的容器启动方法，称为”Static Pod“，允许把Pod的Yaml放在指定目录，当`kubelet`启动时，会自动检查该目录，加载所有的Pod YAML并启动。

kubeadm创建的K8s集群，Master组件的Yaml会被生成在`/etc/kubernetes/manifests`路径下：

- `etcd.yaml`、`kube-apiserver.yaml`、`kube-controller-manager.yaml`、`kube-scheduler.yaml`；



## 材料

[《面向微服务和无服务器计算的下一代云上调度》](./pdf/无服务器计算V6.pptx) 过敏意 上海交通大学
