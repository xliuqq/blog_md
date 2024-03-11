# 分布式块存储方案-[Longhorn](https://longhorn.io/)

> CNCF Incubating Project：Cloud native distributed block storage for Kubernetes. 

轻量级云原生**崩溃一致(`crash-consistent`)**的**分布式块存储**解决方案：

- 支持PV、PVC，支持PV与PVC动态绑定，支持块设备和文件系统两种类型；
- 将块存储划分为Longhorn卷，以便在有或没有云提供商的情况下使用Kubernetes卷；
- 支持不中断卷服务的情况下进行升级；
- 支持备份、快照和恢复；



## 架构

Longhorn 为每个volume创建一个专用的存储控制器（Longhorn Engine），并将volume的数据复制到多个副本上

- Longhorn Manager：以DaemonSet的方式在Longhorn集群的每个节点上运行，负责在k8s中创建和管理卷
- Longhorn Engine： Longhorn Manager创建volume时，会在volume所在的节点创建engine，engine负责数据读写、复制等操作

![how-longhorn-works](pics/how-longhorn-works.svg)



## 原理

`Longhorn CSI driver` 获取块设备(`block device`)，对其进行格式化，然后将其挂载到节点上。然后 kubelet 将设备绑定挂载到 `Kubernetes Pod` 中。这允许 `Pod` 访问 `Longhorn volume`。



## 使用

### 依赖iscsid

> iSCSI是一种存储设备远程映射技术，它可以将一个远程服务器上的存储设备映射到本地，并呈现为一个块设备（大白话就是磁盘）。
>
> - SCSI协议是计算机与外围设备（例如硬盘、光盘等）通信的协议， iSCSI 是通过以太网（TCP）传输SCSI协议

- 集群每个节点已经安装下列软件包，并正常启动：[open-iscsi](https://github.com/open-iscsi/open-iscsi)（iscsid服务）
  - 安装： `yum install iscsi-initiator-utils`


### Helm 安装

```shell
$ helm repo add longhorn https://charts.longhorn.io

$ helm repo update

# 下载包
$ helm fetch longhorn/longhorn

# 解压包
$ tar xf longhorn-*.tgz
```

修改配置文件 `values.yaml`

```yaml
# 看需求设置是否为默认 storageclass（defaultClass），修改pvc策略为 Retain
persistence:
  defaultClass: false
  defaultFsType: ext4           # 默认的文件系统类型
  defaultMkfsParams: ""
  defaultClassReplicaCount: 3
  defaultDataLocality: disabled # best-effort otherwise
  reclaimPolicy: Retain
  migratable: false
  recurringJobSelector:
    enable: false
    jobList: []
  backingImage:
    enable: false
    name: ~
    dataSourceType: ~
    dataSourceParameters: ~
    expectedChecksum: ~
  defaultNodeSelector:
    enable: false # disable by default
    selector: ""
  removeSnapshotsDuringFilesystemTrim: ignored # "enabled" or "disabled" otherwise

# 设置longhorn挂载的本地存储路径，/data/longhorn
defaultSettings:
  backupTarget: ~
  backupTargetCredentialSecret: ~
  allowRecurringJobWhileVolumeDetached: ~
  createDefaultDiskLabeledNodes: ~
  defaultDataPath: /data/longhorn
  
# 修改ui界面的service类型为NodePort
service:
  ui:
    type: NodePort
    nodePort: null
  manager:
    type: ClusterIP
    nodePort: ""
    loadBalancerIP: ""
    loadBalancerSourceRanges: ""
```

部署

```shell
$ helm install longhorn longhorn/longhorn --namespace longhorn-system --create-namespace -f ./values.yaml
```



## 性能测试

基于 [longhorn/kbench: Benchmark your Kubernetes storage. (github.com)](https://github.com/longhorn/kbench) 项目进行性能测试。
