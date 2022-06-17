# 使用

## kubeadm

在宿主机上运行`kubelet`，使用容器部署其它组件。

kubeadm生成的证书文件在Master节点的`/etc/kubernetes/pki`目录下，最主要的证书文件是`ca.crt`和对应的私钥`ca.key`。

Master组件的YAML文件会被生成在`/etc/kubernetes/manifests`路径下；

## kubectl

### apply

部署或者更新

- `kubectl apply -f app.yaml`

### get

- 查看pod服务
  - `kubectl get pod -n cbc-dev | grep router`
- 查询服务pod 和所在的节点
  - `kubectl -n cbc-dev get pod -o wide`
- 查看deployment
  - `kubectl get deployments`
- 查看replicaset
  - `kubectl get rs`
- 查看namespace
  - `kubectl get ns` 


### describe

查看pod详情

- `kubectl -n cbc-dev describe pod podName `

### logs

查看pod 启动日志

- `kubectl -n cbc-dev logs podName `

### exec

进入pod查看

- `kubectl -n cbc-dev exec -it podName bash`

### expose

对deployment提供service

- `kubectl expose deployment webapp`

### delete

删除pods

- `kubectl -n cbc-dev delete -f app.yaml `

### scale

Pod伸缩

- `kubectl scale deployment nginx-deployment --replicas=4`

### edit

编辑Yaml配置

- `kubectl edit deployment/nginx-deployment`

### rollout

回滚版本

- `kubectl rollout undo deployment/nginx-deployment`

查看历史

- `kubectl rollout history deployment/nginx-deployment`

查看滚动更新的状态

- `kubectl rollout status deployment/nginx-deployment`

暂停（批量改配置)，暂停 -> 修改配置 -> 恢复 -> 滚动更新

- `kubectl rollout pause deployment/nginx-deployment`

恢复

- `kubectl rollout resume deployment/nginx-deployment`

