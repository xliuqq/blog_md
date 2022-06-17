# GitOps



## [Flux](https://github.com/fluxcd/flux2)

以**声明方式管理其Kubernetes部署**，**定期轮询存储库来将存储在源代码存储库**中的Kubernetes manifests文件与Kubernetes集群同步。

### 传统的Jenkins + K8s

流程：

- 开发人员创建代码并编写Dockerfile。他们还为应用程序创建Kubernetes manifests和Helm Charts。
- 他们将代码推送到源代码存储库。
- 源代码存储库使用提交后的钩子触发Jenkins构建。
- Jenkins CI流程将构建Docker映像和Helm软件包，并将其推送到依赖仓库。
- 然后，Jenkins CD程序部署helm charts到k8s cluster。

问题：

- Kubernetes **凭据存储**在Jenkins服务器中；
- 可以使用Jenkins创建和更改配置，但**无法使用它删除现有资源**；

### Flux CD

- 团队编写Kubernetes manifests并将其推送到源代码存储库。
- memcached pod存储当前配置。
- Flux定期（默认为五分钟）使用Kubernetes operator轮询存储库以进行更改。Flux[容器](https://cloud.tencent.com/product/tke?from=10680)将其与memcached中的现有配置进行比较。如果检测到更改，它将通过运行一系列kubectl apply/delete命令将配置与集群同步。然后，它将最新的元数据再次存储在memcached存储中。