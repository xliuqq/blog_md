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
- 支持多个K8s环境；



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

### 使用

```shell
kubectl create namespace argocd 
kubectl apply -n argocd -f https://raw.githubusercontent.com/argoproj/argo-cd/stable/manifests/install.yaml
```



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

