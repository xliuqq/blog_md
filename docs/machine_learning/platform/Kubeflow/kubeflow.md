# Kubeflow

> The Machine Learning Toolkit for Kubernetes
>
> Kubeflow is an open, community driven project to make it easy to deploy and manage an ML stack on Kubernetes



## 架构

![An architectural overview of Kubeflow on Kubernetes](pics/kubeflow-overview-platform-diagram.svg)

![An architectural overview of Kubeflow on Kubernetes](https://www.kubeflow.org/docs/images/kubeflow-overview-platform-diagram.svg)

## 组件

### Notebook



### Pipelines

将机器学习的各个阶段进行串联，同时提供了下述能力：

- **任务编排**：通过argo提供workflow的能力，能够实现丰富多样的DAG 工作流，用户可以根据的业务需求定义、管理和复用自己工作流；
- **实验管理**：通过Experiments的能力，能够展示和对比不同实验参数（例如：模型超参）下Pipeline的运行结果，用户根据结果来对工作流任务进行调优；
- **模型追溯**：通过Tracking的能力，能够记录每一次Pipeline运行中每个step的输入和输出信息，用户可以根据记录的内容进行问题排查或模型调优；



<img src="./pics/pipelines-architecture.png" alt="Pipelines architectural diagram" style="zoom:90%;" />



#### 原理

> 提供Python的 SDK 让用户来快速构建符合自己业务场景的Pipeline.

- 通过代码构建 Pipeline 的 py 文件，执行后生成 argo workflow 的 yaml；
- 提供`@func_to_container_op`注解将 python 函数转为 `container`（Argo workflow）；



#### 模块

- Pipeline web server & Pipline Service：

  - 用来创建、管理和展示pipeline、experiments、runs和artifacts等信息；

- Pipeline Persistence Agent：

  - 用来watch pipeline执行的相关信息，并向信息写入到 Mysql 和 ml metada 中；

- orchestration controllers：

  - Kubeflow Pipelines所使用的controller，典型的如Argo Workflow controller用来执行workflow、Scheduler Workflow Controller用来执行定时任务；

- Artifact storage：

  - 存储每次pipeline运行的**input、output和日志**等信息；

  

#### 存储

**MYSQL**：平台运行的信息的存储；

**MINIO**：中间结果数据存储；



#### 数据的读取



#### 数据的传输

通过 Argo 实现。



### Experiments



### Katib

## 使用

### Pipeline

> 



## 安装

### Pipeline

```shell
export PIPELINE_VERSION=1.8.5
kubectl apply -k "github.com/kubeflow/pipelines/manifests/kustomize/cluster-scoped-resources?ref=$PIPELINE_VERSION"
kubectl wait --for condition=established --timeout=60s crd/applications.app.k8s.io
kubectl apply -k "github.com/kubeflow/pipelines/manifests/kustomize/env/dev?ref=$PIPELINE_VERSION"
```

