# [K8s CRD](https://kubernetes.io/docs/tasks/extend-kubernetes/custom-resources/custom-resource-definitions/)

> K8s 的 CRD 的 Yaml 定义，kubebuilder 等三方软件最终也会产生这样的 Yaml 文件定义。

## CustomResourceDefinition

> 复用 K8s 的 Api Server，定义 CRD 和 提供 CRD 的控制器。

创建新的 CustomResourceDefinition（CRD）时，Kubernetes API 服务器会为你所指定的**每个版本生成一个新的 RESTful 资源路径**。

### subresources

k8s 1.11 版本，支持 subresources 配置，包括 `status`和`scale`两类。

### 校验机制

基于 OpenAPI v3 schema 的校验机制，配置 CRD 中字段配置的合法性。

### 自定义显式列

用于 `kubectl get crd crd-name`的展示

### Finalizer

创建 CRD 对象时，设置 Finalizer，CRD controller 执行相应的清理函数，再删除该 Finalizer 字段，实现异步的删除前（Pre-delete）回调。

## [CRD 的多个版本](https://kubernetes.io/zh-cn/docs/tasks/extend-kubernetes/custom-resources/custom-resource-definition-versioning/)



## 示例

### 创建 CRD

将下面的 CustomResourceDefinition 保存到 `resourcedefinition.yaml` 文件

```yaml
apiVersion: apiextensions.k8s.io/v1
kind: CustomResourceDefinition
metadata:
  name: crontabs.stable.example.com
spec:
  group: stable.example.com
  versions:
    - name: v1
      served: true
      storage: true
      schema:
        # openAPIV3Schema 是验证自定义对象的模式。
        openAPIV3Schema:
          type: object
          properties:
            spec:
              type: object
              properties:
                cronSpec:
                  type: string
                  pattern: '^(\d+|\*)(/\d+)?(\s+(\d+|\*)(/\d+)?){4}$'
                image:
                  type: string
                replicas:
                  type: integer
                  minimum: 1
                  maximum: 10
            status:
              type: object
              properties:
                replicas:
                  type: integer
                labelSelector:
                  type: string
      # subresources 描述定制资源的子资源
      subresources:
        # status 启用 status 子资源
        status: {}
        # scale 启用 scale 子资源
        scale:
          # specReplicasPath 定义定制资源中对应 scale.spec.replicas 的 JSON 路径
          specReplicasPath: .spec.replicas
          # statusReplicasPath 定义定制资源中对应 scale.status.replicas 的 JSON 路径 
          statusReplicasPath: .status.replicas
          # labelSelectorPath  定义定制资源中对应 scale.status.selector 的 JSON 路径 
          labelSelectorPath: .status.labelSelector
  scope: Namespaced
  names:
    plural: crontabs
    singular: crontab
    kind: CronTab
    shortNames:
    - ct
```

创建

```shell
kubectl apply -f resourcedefinition.yaml
```

一个新的受名字空间约束的 RESTful API 端点会被创建在：

```
/apis/stable.example.com/v1/namespaces/*/crontabs/...
```

创建定制对象（Custom Objects）

下面的 YAML 保存到 `my-crontab.yaml`：

```yaml
apiVersion: "stable.example.com/v1"
kind: CronTab
metadata:
  name: my-new-cron-object
spec:
  cronSpec: "* * * * */5"
  image: my-awesome-cron-image
  replicas: 2
```

并执行创建命令：

```shell
kubectl apply -f my-crontab.yaml
```

使用 kubectl 来管理你的 CronTab 对象了。例如：

```shell
kubectl get crontab
```

应该会输出如下列表：

```none
NAME                 AGE
my-new-cron-object   6s
```

### 创建 CRD Controller
