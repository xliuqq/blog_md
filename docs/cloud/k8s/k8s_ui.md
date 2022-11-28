# K8s UI

## Dashborad

### 部署

```bash
kubectl apply -f https://raw.githubusercontent.com/kubernetes/dashboard/v2.3.1/aio/deploy/recommended.yaml
# 查看
kubectl get pods -n kubernetes-dashboard
# 配置代理，接受所有地址的访问
kubectl  proxy --address="172.16.2.131" --port=8001 --accept-hosts="^.*"
```

```bash
# 查看kubernetes-dashboard的token
kubectl -n kubernetes-dashboard describe secret $(kubectl -n kubernetes-dashboard get secret | grep kubernetes-dashboard | awk '{print $1}')
```

该token可能因为kubernetes-dashboard 这个账户的角色权限不够，界面数据出问题。

**创建dashboard-adminuser.yaml**（未尝试）

```yaml
cat > dashboard-adminuser.yaml << EOF
apiVersion: v1
kind: ServiceAccount
metadata:
  name: admin-user
  namespace: kubernetes-dashboard

---
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRoleBinding
metadata:
  name: admin-user
roleRef:
  apiGroup: rbac.authorization.k8s.io
  kind: ClusterRole
  name: cluster-admin
subjects:
- kind: ServiceAccount
  name: admin-user
  namespace: kubernetes-dashboard  
EOF
```



```bash
# 创建了一个叫 admin-user 的服务账号，并放在kubernetes-dashboard 命名空间下，并将 cluster-admin 角色绑定到admin-user账户，这样admin-user账户就有了管理员的权限。默认情况下，kubeadm创建集群时已经创建了cluster-admin角色，我们直接绑定即可。
kubectl apply -f dashboard-adminuser.yaml
# 查看admin-user账户的token
kubectl -n kubernetes-dashboard describe secret $(kubectl -n kubernetes-dashboard get secret | grep admin-user | awk '{print $1}')

```

