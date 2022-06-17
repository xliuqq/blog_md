# 配置

## Taint/Toleration

默认情况下，Master节点是不允许运行用户Pod的。通过`Taint/Toleration`机制，可以在Master节点部署用户Pods。

**原理**：一旦某个节点被加上一个Taint，即”染上污点“，那么 运行，除非个别节点声明自己能够容忍”污点”，即声明了“Toleration”。

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

节点删除“污点”的命令：

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



### Generation

- 对所有变更请求，除非改变是针对 `.metadata` 或 `.status`，`.metadata.generation` 的取值都会增加。

### Finalizers

每当删除 namespace 或 pod 等一些 Kubernetes 资源时，有时资源状态会卡在 `Terminating`，很长时间无法删除，甚至有时增加 `--force` flag 之后还是无法正常删除。这时就需要 `edit` 该资源，将 `finalizers` 字段设置为 []，之后 Kubernetes 资源就正常删除了。

字段属于 **Kubernetes GC 垃圾收集器**，是一种删除拦截机制，能够让控制器实现**异步的删除前（Pre-delete）回调**，存在于**资源对象的 Meta**中。

对带有 Finalizer 的对象的**第一个删除请求会为其 `metadata.deletionTimestamp` 设置一个值，但不会真的删除对象**。一旦此值被设置，finalizers 列表中的值就**只能**被移除。

当 `metadata.deletionTimestamp` 字段被设置时，负责监测该对象的各个控制器会通过**轮询**对该对象的更新请求来执行它们所要处理的所有 Finalizer。当所有 Finalizer 都被执行过，资源被删除。

`metadata.deletionGracePeriodSeconds` 的取值控制对更新的轮询周期。

每个控制器要负责将其 Finalizer 从列表中去除。

每执行完一个就从 `finalizers` 中移除一个，直到 `finalizers` 为空，之后其宿主资源才会被真正的删除。

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

