# [Argo](https://argoproj.github.io/)

> Open source tools for **Kubernetes** to run workflows, manage clusters, and do GitOps right.

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
- Git 仓库中特定目录下的YAML/json manifests，包括 jsonnnet 文件；
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
# 创建argocd名空间
kubectl create namespace argocd 
# 执行apply安装
kubectl apply -n argocd -f https://raw.githubusercontent.com/argoproj/argo-cd/stable/manifests/install.yaml

# 查看安装镜像下载状态
kubectl describe pod -n argocd |grep Image: |sort |uniq -c

# 开启UI的Nodeport
kubectl -n argocd expose deployments/argocd-server --type="NodePort" --port=8080 --name=argocd-server-nodeport

# 获取密码，用户名为：admin
kubectl -n argocd get secret argocd-initial-admin-secret -o jsonpath="{.data.password}" | base64 -d|xargs -n1 echo
```

默认会安装一个自签名的证书。



#### helm安装

https://github.com/argoproj/argo-helm/tree/main/charts/argo-cd



### HA 安装（TODO）



### CLI 操作

> Argo CD 提供UI界面，以及命令行操作。

#### CLI安装

```shell
curl -sSL -o argocd-linux-amd64 https://github.com/argoproj/argo-cd/releases/latest/download/argocd-linux-amd64
sudo install -m 555 argocd-linux-amd64 /usr/local/bin/argocd
rm argocd-linux-amd64
```

#### CLI 登录

```shell
# 登录
argocd login <ARGOCD_SERVER>  --username admin --password admin123456 --insecure

# 修改密码，完成之后，需要删除`argocd-initial-admin-secret`(仅用于保存初始密码，会根据新的密码自动创建新的)
argocd account update-password
```

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

> 默认情况下只能添加argocd所在的K8S集群，添加k8s集群无法在web控制台操作，只能通过命令行操作。

1. 目标 K8s 集群获取 context 信息

   ```shell
   # 在目标k8s集群执行 
   $ kubectl config get-contexts
   CURRENT   NAME                          CLUSTER      AUTHINFO           NAMESPACE
   *         kubernetes-admin@kubernetes   kubernetes   kubernetes-admin
   
   # 将 kube config 文件发送到 argocd 机器，注意将里面的域名信息改为 ip 地址
   $ scp /root/.kube/config 172.16.2.134:/argocd/prod_k8s_config
   ```

2.  在 argocd 的机器上通过命令行新增集群

   ```shell
   $ argocd cluster add kubernetes-admin@kubernetes  --kubeconfig /argocd/prod_k8s_onfig  --name prod
   ```

   



### 配置

#### Git Webhook

Argo CD 默认每3分钟检测Git仓库中manifests的变化，可以通过**webhook event 监听变更，实时监测**：

- 配置地址为：`https://argocd.server.url/api/webhook`



#### Helm (TODO)



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

