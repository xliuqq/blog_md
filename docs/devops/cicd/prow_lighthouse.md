# ChatOps 

## K8s原生 CI/CD 系统 Prow 简介

> [Prow](https://github.com/kubernetes/test-infra/tree/master/prow) 是一个 Kubernetes 原生 CI/CD 系统，Kubernetes、Istio 等项目都使用 *Prow* 实现开源协同。 

[Jenkins X](https://jenkins-x.io/) 使用 Prow 作为 Serverless Jenkins 的一部分。

目前仅支持GitHub。[Gitlab集成的Prorosal](https://github.com/kubernetes/test-infra/issues/10146)；

## 使用

需要创建一个github机器人账号，需要k8s集群以及硬件资源，网络资源，创建钩子，部署，二次开发。

[官方参考](https://github.com/kubernetes/test-infra/blob/master/prow/getting_started_deploy.md)

[示例QuickStart](https://github.com/zhangsean/prow-quickstart)

## [Jenkins-x Lighthouse](https://github.com/jenkins-x/lighthouse)

Lighthouse 是一个**基于 webhooks 的轻量级 ChatOps 工具** , 通过 Git 仓库的 webhooks 可以**触发 Jenkins X 流水线** 、Tekton 流水线 、Jenkins 任务, 支持 GitHub、GitHub Enterprise、BitBucket Server 和 GitLab。

