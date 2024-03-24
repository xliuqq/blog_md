# Device Plugin

## Extended Resource

扩展节点的资源

```shell
# 开启kube proxy，则可以通过 localhost 请求 APIServer
kubectl proxy

# 给节点添加 nvidia.com/gpu 的资源
curl --header "Content-Type: application/json-patch+json" \
--request PATCH \
--data '[{"op": "add", "path": "/status/capacity/nvidia.com/gpu", "value": "4"}]' \
http://localhost:8001/api/v1/nodes/<your-node-name>/status
```



## API

```protobuf
service DevicePlugin {
    rpc ListAndWatch(Empty) returns (stream ListAndWatchResponse) {}
    rpc Allocate(AllocateRequest) returns (AllocateResponse) {}
}
```

- device plugin 通过`ListAndWatch`的API定期向 kubelet 汇报该节点上的 GPU  的信息，kubelet 通过心跳向 API Server 以 Extended Resource的形式加上GPU的数量信息；
- Pod 申请 nvidia.com/gpu 资源，调度器分配满足的节点；
- 该节点的kubelet发现 Pod 需要GPU时，从持有的GPU列表为容器分配 GPU（kubelet 向本机的 Device Plugin 发起 Allocate 请求）；
- Device plugin 根据 kubelet 传来的设备 ID，查找设备对应的设备路径和驱动目录（如 Nvidia 定期访问 nvidia-docker 插件 获取本机GPU信息）。



## GPU 使用

### [NVIDIA device plugin](https://github.com/NVIDIA/k8s-device-plugin)

> *if you don't request GPUs when using the device plugin with NVIDIA images all the GPUs on the machine will be exposed inside your container.*

Nvidia 官方实现[k8s device plugin](./k8s_device.md)，作为 K8s Daemonset：

- 容器使用集群上节点的GPU；
- 跟踪GPU的健康状态；


#### 安装

> With the release of Docker 19.03, usage of `nvidia-docker2` packages is deprecated since NVIDIA GPUs are now natively supported as devices in the Docker runtime ( --gpus ).

- 宿主机需要安装 nvidia driver；
- 支持 docker / containerd / CRI-O；

##### nvidia-container-toolkit

- nvidia-container-toolkit >= 1.7.0 (nvidia-docker, nvidia-docker2被弃用)

  ```shell
  distribution=$(. /etc/os-release;echo $ID$VERSION_ID)
  curl -s -L https://nvidia.github.io/libnvidia-container/gpgkey | sudo apt-key add -
  curl -s -L https://nvidia.github.io/libnvidia-container/$distribution/libnvidia-container.list | sudo tee /etc/apt/sources.list.d/libnvidia-container.list
  
  sudo apt-get update && sudo apt-get install -y nvidia-container-toolkit
  ```

##### nvidia-container-runtime

- `nvidia-container-runtime`被配置为默认的 low-level runtime；

docker：  `/etc/docker/daemon.json`

  ```json
{
    "default-runtime": "nvidia",
    "runtimes": {
        "nvidia": {
            "path": "/usr/bin/nvidia-container-runtime",
            "runtimeArgs": []
        }
    }
}
// sudo systemctl restart docker
  ```

containerd：  `/etc/containerd/config.toml`

  ```toml
version = 2
[plugins]
  [plugins."io.containerd.grpc.v1.cri"]
    [plugins."io.containerd.grpc.v1.cri".containerd]
      default_runtime_name = "nvidia"

      [plugins."io.containerd.grpc.v1.cri".containerd.runtimes]
        [plugins."io.containerd.grpc.v1.cri".containerd.runtimes.nvidia]
          privileged_without_host_devices = false
          runtime_engine = ""
          runtime_root = ""
          runtime_type = "io.containerd.runc.v2"
          [plugins."io.containerd.grpc.v1.cri".containerd.runtimes.nvidia.options]
            BinaryName = "/usr/bin/nvidia-container-runtime"
            
# sudo systemctl restart containerd
  ```

##### GPU Plugin

Yaml 安装

```shell
# 最新版见 https://github.com/NVIDIA/k8s-device-plugin/releases/
$ kubectl create -f https://raw.githubusercontent.com/NVIDIA/k8s-device-plugin/v0.14.4/nvidia-device-plugin.yml
```

Helm 安装

```shell
$ helm repo add nvdp https://nvidia.github.io/k8s-device-plugin
$ helm repo update
$ helm search repo nvdp --devel
NAME                     	  CHART VERSION  APP VERSION	DESCRIPTION
nvdp/nvidia-device-plugin	  0.12.3         0.12.3         A Helm chart for ...

# installation command without any options is then:
$ helm upgrade -i nvdp nvdp/nvidia-device-plugin \
  --namespace nvidia-device-plugin \
  --create-namespace \
  --version 0.12.3

```



#### 测试

```shell
$ cat <<EOF | kubectl apply -f -
apiVersion: v1
kind: Pod
metadata:
  name: gpu-pod
spec:
  restartPolicy: Never
  containers:
    - name: cuda-container
      image: nvcr.io/nvidia/k8s/cuda-sample:vectoradd-cuda10.2
      resources:
        limits:
          nvidia.com/gpu: 1 # requesting 1 GPU
  tolerations:
  - key: nvidia.com/gpu
    operator: Exists
    effect: NoSchedule
EOF
```

#### 配置

| Flag                     | Envvar                  | Default Value |
| ------------------------ | ----------------------- | ------------- |
| `--mig-strategy`         | `$MIG_STRATEGY`         | `"none"`      |
| `--fail-on-init-error`   | `$FAIL_ON_INIT_ERROR`   | `true`        |
| `--nvidia-driver-root`   | `$NVIDIA_DRIVER_ROOT`   | `"/"`         |
| `--pass-device-specs`    | `$PASS_DEVICE_SPECS`    | `false`       |
| `--device-list-strategy` | `$DEVICE_LIST_STRATEGY` | `"envvar"`    |
| `--device-id-strategy`   | `$DEVICE_ID_STRATEGY`   | `"uuid"`      |
| `--config-file`          | `$CONFIG_FILE`          | `""`          |

`--pass-device-specs`  ：是否与 [CPUManager(cpuset 减少cpu切换)](https://kubernetes.io/zh-cn/docs/tasks/administer-cluster/cpu-management-policies/#static-policy-options) 一起使用

##### helm 安装前配置

https://github.com/NVIDIA/k8s-device-plugin#configuring-the-device-plugins-helm-chart

