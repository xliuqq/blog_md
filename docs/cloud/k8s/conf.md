# 配置

## Taint/Toleration

默认情况下，Master节点是不允许运行用户Pod的。通过`Taint/Toleration`机制，可以在Master节点部署用户Pods。

**原理**：节点被加上一个Taint（染上“污点“），如果节点未声明自己能够容忍该”污点”（Toleration），则Pod不能在上面运行。

节点加上“污点”的命令：

- **NoSchedule意味着该Taint只会在调度新的Pod时产生作用，不会影响node1上已经运行的Pods**

```bash
$ kubectl taint nodes node1 foo=bar:NoSchedule
```

Pod声明Toleartion：

```yaml
# 能够容忍所有键值对为foo=bar的Taint
apiVersion: v1
kind: Pod
...
spec:
  toleartions:
    - key: "foo"
      operation: "Equal"
      value: "bar"
      effect: "NoSchedule"
```

节点删除“污点”的命令：让Master节点可以运行Pod

```bash
# 删除所有节点的node-role.kubernetes.io/master的Taint（最后的减号表示删除）
$ kubectl taint nodes --all node-role.kubernetes.io/master -
```

## Yaml说明

资源yaml的构成：

- TypeMeta：Kind，ApiVersion；
- ObjectMeta：即对应的.metadata；
- Spec：对应的 .spec；
- Status：对应的 .status；

### ResourceVersion

全局唯一的版本号（`metadata.resourceVersion`）。

**每个资源对象从创建开始就会有一个版本号，而后每次被修改（不管是 update 还是 patch 修改），版本号都会发生变化。**

[官方文档](https://kubernetes.io/docs/reference/using-api/api-concepts/#resource-versions)告诉我们，这个版本号是一个 K8s 的内部机制，用户不应该假设它是一个数字或者通过比较两个版本号大小来确定资源对象的新旧，唯一能做的就是**通过比较版本号相等来确定对象是否是同一个版本**（即是否发生了变化）。

- 如果两个用户同时对一个资源对象做 update，不管操作的是对象中同一个字段还是不同字段，都存在版本控制的机制确保两个用户的 update 请求不会发生覆盖。

### Generation

- 对所有变更请求，除非改变是针对 `.metadata` 或 `.status`，`.metadata.generation` 的取值都会增加。

### Finalizers

> `Finalizers` 是由字符串组成的数组，当 `Finalizers` 字段中存在元素时，相关资源不允许被删除

每当删除 namespace 或 pod 等一些 Kubernetes 资源时，有时资源状态会卡在 `Terminating`，很长时间无法删除，甚至有时增加 `--force` flag 之后还是无法正常删除。这时就需要 `edit` 该资源，将 `finalizers` 字段设置为 []，之后 Kubernetes 资源就正常删除了。

字段属于 **Kubernetes GC 垃圾收集器**，是一种删除拦截机制，能够让控制器实现**异步的删除前（Pre-delete）回调**，存在于**资源对象的 Meta**中。

对带有 Finalizer 的对象的**第一个删除请求会为其 `metadata.deletionTimestamp` 设置一个值，但不会真的删除对象**。一旦此值被设置，finalizers 列表中的值就**只能**被移除。

当 `metadata.deletionTimestamp` 字段被设置时，负责监测该对象的各个控制器会通过**轮询**对该对象的更新请求来执行它们所要处理的所有 Finalizer。当所有 Finalizer 都被执行过，资源被删除。

`metadata.deletionGracePeriodSeconds` 的取值控制对更新的轮询周期。

每个控制器要负责将其 Finalizer 从列表中去除。

每执行完一个就从 `finalizers` 中移除一个，直到 `finalizers` 为空，之后其宿主资源才会被真正的删除。

### Owner References 属主与附属

ReplicaSet 是一组 Pod 的属主，具有属主的对象是属主的附属（Dependent）。附属对象有一个 `metadata.ownerReferences` 字段，用于引用其属主对象。在 Kubernetes 中不允许跨 namespace 指定属主。



### delete 策略

**Foreground策略**：先**删除附属对象，再删除属主对象**。将对象的`metadata.finalizers`字段值设置为`foregroundDeletion`，控制器需要主动处理`foregroundDeletion`的finalizers。

**Background策略（默认）**：删除一个对象同时会删除它的附属对象。

**Orphan策略**：不会自动删除它的附属对象，这些残留的依赖被称为原对象的孤儿对象。

## SecurityContext 

三种级别的设置：

- Container级别：Container-level Security Context
- 单Pod内所有容器和Volume：Pod-level Security Context
- 集群内部所有Pod和Volume：Pod Security Policies

配置安全上下文可以

- 访问权限控制：指定容器中运行进程的用户和用户组（通过uid和gid）；
- 阻止容器使用 root 户运行（容器的默认运行用户通常在其镜像中指定，所以可能需要阻止容器 root 用户运行；
- 使用特权模式运行容器，使其对宿主节点的内核具有完全的访问权限；
- 与以上相反，通过添加或禁用内核功能，配置细粒度的内核访问权限；
- 设 SELinux C Security aced Linux 安全增强型 Linux ）边项，加强对容器的限制；
- 阻止进程写入容器的根文件系统



```bash
kubectl explain 'pods.spec.securityContext'
```



Pod的设置：

```yaml
apiVersion: v1 
kind: Pod 
metadata: 
  name: pod-as-user-guest 
spec: 
  containers: 
  - name: main
    image: alpine 
    command: ["/bin/sleep","99999"]
    securityContext: 
      # 特定用户，只能指定用户Id
      runAsUser: 405
      # 特定用户组
      runAsGroup: 1000
      #
      fsGroup:
      
      # 阻止容器使用 root 用户运行
      runAsNonRoot: true
```

## native configuration management

kustomize 最简实践（TODO）

https://www.jianshu.com/p/837d7ae77818



## 修改NodePort范围

>  默认范围是 30000-32767。

使用 kubeadm 安装 K8S 集群的情况下，修改`/etc/kubernetes/manifests/kube-apiserver.yaml`文件，向其中添加 `--service-node-port-range=20000-22767` （定义需要的端口范围）

重启kube-apiserver

```bash
# 获得 apiserver 的 pod 名字
export apiserver_pods=$(kubectl get pods --selector=component=kube-apiserver -n kube-system --output=jsonpath={.items..metadata.name})
# 删除 apiserver 的 pod
kubectl delete pod $apiserver_pods -n kube-system
```

验证结果

- 执行以下命令查看相关pod

```bash
kubectl describe pod $apiserver_pods -n kube-system
```

注意

- 对于已经创建的NodePort类型的Service，需要删除重新创建
- 如果集群有多个 Master 节点，需要逐个修改每个节点上的 /etc/kubernetes/manifests/kube-apiserver.yaml 文件，并重启 apiserver
