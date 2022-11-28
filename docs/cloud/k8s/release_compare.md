# K8s的可选发行版

## k8s/MiniKube（开发环境）

> minikube quickly sets up a local Kubernetes cluster on macOS, Linux, and Windows. We proudly focus on helping **application developers and new Kubernetes users**.

- 基于 VM/Container 启动的 K8s 集群；
- 支持多节点集群（在一个物理节点启动两个VM组成K8s），而不是多个物理节点组成K8s集群；

### 安装

#### Windows

https://minikube.sigs.k8s.io/docs/start/

Windows配置Hyper -V

安装 Hyper-V

> 【控制面板】->【程序】->【打开和关闭 windows 功能】-> 勾选 Hyper-V 的选项（包括所有子项）-> 重启电脑

启用 Hyper-V

```powershell
Enable-WindowsOptionalFeature -Online -FeatureName Microsoft-Hyper-V -All
```



```shell
minikube start --driver=hyperv 
# To make hyperv the default driver:
minikube config set driver hyperv
```



```bash
minikube start --disk-size="10g"  --image-mirror-country="cn"  --image-repository="registry.cn-hangzhou.aliyuncs.com/google_containers"
```



## Kind

> Kubernetes IN Docker - local clusters for testing Kubernetes.
>
> kind is a tool for running local Kubernetes clusters using Docker container "nodes". kind was primarily designed for testing Kubernetes itself, but may be used for local development or CI.

测试环境

## [MicroK8s](https://github.com/canonical/microk8s)

> **MicroK8s** is a low-ops, minimal production Kubernetes.
>
> MicroK8s is a small, fast, single-package Kubernetes for developers, IoT and edge.



## [K3s](https://github.com/k3s-io/k3s/)

> Lightweight Kubernetes. The certified Kubernetes distribution built for IoT & Edge computing

