# 配置





## 本地磁盘限制

> 防止将宿主机的磁盘撑满，导致宿主机不可用。

### ephemeral-storage

ephemeral-storage 包括：

- `emptyDir` volumes, except *tmpfs* `emptyDir` volumes
- directories holding node-level logs
- writeable container layers

> 在每个Kubernetes的节点上，kubelet的根目录(默认是/var/lib/kubelet)和日志目录(/var/log)保存在节点的主分区上，这个分区同时也会被**Pod的EmptyDir类型的volume、容器日志、镜像的层、容器的可写层所占用**。
>
> - kubelet会统计当前节点的主分区的可分配的磁盘资源，或者可以覆盖节点上kubelet的配置来自定义可分配的资源。
> - 在创建Pod时会根据存储需求调度到满足存储的节点，在Pod使用超过限制的存储时会对其做**驱逐**的处理来保证不会耗尽节点上的磁盘空间。
>

注意事项：

-  如果k8s的根目录（kubelet root-dir）跟容器镜像存储根目录（/var/lib/{docker}）是单独挂载的磁盘（非root盘），则通过 kubectl 查看 node 的 ephemeral-storage 只会显示根分区(/) 的存储空间。
  - The kubelet will only track the root filesystem for ephemeral storage. OS layouts that mount a separate disk to `/var/lib/kubelet` or `/var/lib/containers` will not report ephemeral storage correctly.
  - 在申请资源的时候，可能会出问题（因为总的资源量结果不对）
  
- K8s 1.22 版本及之后，允许[**容器镜像存储根目录（/var/lib/{docker}）单独挂载盘**](https://kubernetes.io/docs/concepts/configuration/manage-resources-containers/#configurations-for-local-ephemeral-storage)，如果镜像可写层超过限制，可以正常被驱除；

使用示例 Pod：

```yaml
resources:
  requests:
    cpu: 1
    memory: 2048Mi
    # k8s 1.8开始引入的特性，限制容器存储空间的使用
    ephemeral-storage: 2Gi
  limits:
    cpu: 2
    memory: 2048Mi
    ephemeral-storage: 5Gi
```

**当ephemeral-storage超出限制时，kublet会驱除当前容器**

> **Pod 内的重启不会被重启**（不会受 restartPolicy 影响，也不会受存活等探针影响），因此需要配合 deployment 等使用，由 deployment 重新创建 Pod；
>
> - 已经 restart 过，但是有错误信息，`reason`状态导致不会继续重启（k8s 1.22版本），参考[Pod驱除](https://kubernetes.io/zh-cn/docs/concepts/scheduling-eviction/node-pressure-eviction/)；

- `get pods -o yaml`信息如下：

```yaml
status:
  - lastProbeTime: null
    lastTransitionTime: "2024-02-04T09:31:31Z"
    reason: PodFailed
    status: "False"
    type: Ready
  - ...
  containerStatuses:
  - image: nginx
    ...
    ready: false
    restartCount: 1
    started: false
    state:
      terminated:
        exitCode: 137
        finishedAt: null
        message: The container could not be located when the pod was terminated
        reason: ContainerStatusUnknown
        startedAt: null
  message: 'Pod ephemeral local storage usage exceeds the total limit of containers 1Gi. '
  phase: Failed
  qosClass: Burstable
  reason: Evicted
```

查看事件时如下：

```shell
  Warning  Evicted    27s    kubelet            Pod ephemeral local storage usage exceeds the total limit of containers 1Gi.
  Normal   Killing    27s    kubelet            Stopping container nginx
```



