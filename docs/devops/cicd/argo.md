# [Argo](https://argoproj.github.io/)

> Open source tools for Kubernetes to run workflows, manage clusters, and do GitOps right.

## 组件

- [Argo Workflows](https://github.com/argoproj/argo-workflows) - Container-native Workflow Engine（在`工作流`引擎中进行介绍）
- [Argo CD](https://github.com/argoproj/argo-cd) - Declarative GitOps Continuous Delivery
- [Argo Events](https://github.com/argoproj/argo-events) - Event-based Dependency Manager
- [Argo Rollouts](https://github.com/argoproj/argo-rollouts) - Progressive Delivery with support for Canary and Blue Green deployment strategies



## Argo CD

> Application definitions, configurations, and environments should be declarative and version controlled. 
>
> Application deployment and lifecycle management should be automated, auditable, and easy to understand.



### 功能

- 支持多种多个 Git 源仓库；
- 能够管理和部署到多个集群；
- 用于授权的多租户和 RBAC 策略；
- 应用资源健康状态分析；
- 自动配置偏差检测和可视化；
- 自动或手动将应用程序同步到所需状态；
- 提供应用程序活动的实时视图的 Web UI；
- 用于自动化和 CI 集成的 CLI；
- Webhook 集成（GitHub、BitBucket、GitLab）；
- 应用程序事件和 API 调用的审计跟踪；





- ？如何获取镜像，进行构建；



支持的k8s manifests方式：

- kustomize 应用；
- helm charts；
- jsonnnet 文件；
- 特定目录下的YAML/json manifests；
- 配置为配置管理插件的任何自定义配置管理工具



### 架构
- 作为`kubernetes controller`；

<img src="pics/argocd_architecture.png" alt="Argo CD Architecture" style="zoom: 80%;" />

***API Server***： a gRPC/REST server；

- invoking of application operations
- application management and status reporting
- listener/forwarder for Git webhook events

***Repository Server***：an internal service which maintains a local cache of the Git repository holding the application manifests；

***Application Controller***：Kubernetes controller which continuously monitors running applications and compares the current, live state against the desired target state

### 安装

依赖：

- `kubectl` ；
- kubeconfig 文件（默认`~/.kube/config`）；
- CoreDNS；

#### yaml安装

```shell
kubectl create namespace argocd 
kubectl apply -n argocd -f https://raw.githubusercontent.com/argoproj/argo-cd/stable/manifests/install.yaml
```

自签名证书配置：

- 

外部访问：

- 

初始密码在`argocd-initial-admin-secret`中（获取密码后可以删除）：

```shell
kubectl -n argocd get secret argocd-initial-admin-secret -o jsonpath="{.data.password}" | base64 -d; echo
```

#### helm安装

https://github.com/argoproj/argo-helm/tree/main/charts/argo-cd

### CLI 操作

#### 创建项目

```shell
# set the current namespace to argocd 
kubectl config set-context --current --namespace=argocd

# Create the example guestbook application with the following command
argocd app create guestbook --repo https://github.com/argoproj/argocd-example-apps.git --path guestbook --dest-server https://kubernetes.default.svc --dest-namespace default
```

#### 同步项目

`argocd app sync guestbook`



### 多集群支持

- 默认情况下只能添加argocd所在的K8S集群，添加k8s集群无法在web控制台操作。



### 配置

#### Git Webhook

Argo CD 默认每3分钟检测Git仓库中manifests的变化，可以通过webhook event 监听变更，实时监测：

- 

## [Argo Events](https://github.com/argoproj/argo-events)

> The Event-Based Dependency Manager for Kubernetes



### Triggers

1. Argo Workflows
2. Standard K8s Objects
3. HTTP Requests / Serverless Workloads (OpenFaaS, Kubeless, KNative etc.)
4. AWS Lambda
5. NATS Messages
6. Kafka Messages
7. Slack Notifications
8. Azure Event Hubs Messages
9. Argo Rollouts
10. Custom Trigger / Build Your Own Trigger
11. Apache OpenWhisk
12. Log Trigger

