# Kubelet

Kubernetes中有一种特殊的容器启动方法，称为”Static Pod“，允许把Pod的Yaml放在指定目录，当`kubelet`启动时，会自动检查该目录，加载所有的Pod YAML并启动。

kubeadm创建的K8s集群，Master组件的Yaml会被生成在`/etc/kubernetes/manifests`路径下：

- `etcd.yaml`、`kube-apiserver.yaml`、`kube-controller-manager.yaml`、`kube-scheduler.yaml`；