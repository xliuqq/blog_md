# Prow 

> [Prow](https://github.com/kubernetes/test-infra/tree/master/prow) 是一个 Kubernetes 原生 CI/CD 系统，Kubernetes、Istio 等项目都使用 *Prow* 实现开源协同。 

目前仅支持GitHub。[Gitlab集成的Prorosal](https://github.com/kubernetes/test-infra/issues/10146)；

## 使用

需要创建一个github机器人账号，需要k8s集群以及硬件资源，网络资源，创建钩子，部署，二次开发。

[官方参考](https://github.com/kubernetes/test-infra/blob/master/prow/getting_started_deploy.md)

[示例QuickStart](https://github.com/zhangsean/prow-quickstart)

