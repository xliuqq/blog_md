# 集群安全机制

## API Server 认证

K8s集群中所有资源访问和变更都是通过k8s API Server的REST API实现，

认证（Authentication）：识别客户端的身份；

### API 访问方式

K8s API的访问方式分类：

- 证书方式访问的普通用户或进程，包括运维人员、kubectl、kubelets等进程；
- Service Account方式访问的K8s的内部服务进程；
- 匿名方式访问的进程。

### API 认证方式

- HTTPS 证书认证：默认，基于**CA根证书签名的双向数字证书**认证方式；
- HTTP Bear Token认证：通过**Bearer Token**识别合法用户，指定存储的文件（存储token对应的用户信息，csv格式）；
- OpenID Connect Token认证：通过第三方OIDC协议进行认证；
- Webhook Token认证：通过**外部Webhook服务**进行认证；
- Authentication Proxy认证：通过认证代理程序进行认证；

## API Server 授权

### 授权策略

默认为`--authorization-mode=Node,RBAC`

- ~~AllowDeny：拒绝所有，仅用于测试；~~
- ~~AlwaysAllow：允许所有，集群不需要授权时使用；~~
- ~~ABAC：基于属性的访问控制；~~
- **RBAC**：基于角色的访问控制；
- **Webhook**：基于外部的REST服务进行授权；
- **Node**：对kubelet进行授权的特殊模式；

### RBAC

资源对象：Role（受限于命名空间）、ClusterRole（全局）、RoleBinding、ClusterRoleBinding；

- Role或ClusterRole与RoleBinding或ClusterRoleBinding**绑定之后，则Role/ClusterRole无法修改**；
- K8s内置ClusterRole，包括admin，view，edit等；

Rules的使用：

- `apiGroups`：“”（Core），"extensions", "apps", "batch" 等；
- `resources`：“services”, “endpoints”, “pods“，"deployments“，“jobs”，“configmaps”，“nodes”，“rolebindings”，“clusterroles” 等；
- `verbs`：create、delete、deletecollection、get、list、patch、update、watch、bind；
- `resourceNames`：数据权限，**限制特定实例名称**有权限，可用于`get,delete,update,patch`，但对`list`, `watch`, `create`, `deletecollection`操作无效；

主体绑定：

- `User`：字符串标识，通常应该在客户端CA证书中进行设置，K8s内置系统级别的用户/用户组，以"system:"开头；
- ``Group`：与用户名类似，通常应该在客户端CA证书中进行设置，不以"system:"为前缀；
- `Service Account`：用户和所属的组名，会被k8s设置为以"system:serviceaccount"为前缀的名称；

示例：

```yaml
kind: Role
apiVersion: rbac.authorization.k8s.io/v1
metadata:
  namespace: default
  name: pod-reader
rules:
- apiGroups: [""]
  resources: ["pods"]
  resourceNames: ["test-app"]
  verbs: ["get", "watch", "list"]
---
kind: RoleBinding
apiVersion: rbac.authorization.k8s.io/v1
metadata:
  name: read-pods
  namespace: default
subjects:
- kind: User
  name: mark
  apiGroup: rbac.authorization.k8s.io  # 固定的字段指
roleRef:
  kind: Role
  name: pod-reader
  apiGroup: rbac.authorization.k8s.io
```

## Admission Control

准入控制器的插件列表，支持用户自定义扩展。

- [**Mutating Admission Webhook，Validating Admission Webhook**](./k8s.md#Webhook)