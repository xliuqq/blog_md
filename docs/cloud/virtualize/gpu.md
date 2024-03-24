# GPU 使用

docker 原生可以通过 ``--device /dev/nvidia0:/dev/nvidia0`的方式，支持GPU使用和隔离，但是无法**对GPU 可用性做判断**。

## Nvidia GPU

[安装使用](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/latest/install-guide.html)

### 原理

[libnvidia-container](https://github.com/NVIDIA/libnvidia-container)

- 提供一个库和简单的CLI工具，以**实现在容器当中支持使用GPU设备的目标**。

[nvidia-container-toolkit](https://github.com/NVIDIA/nvidia-container-toolkit)

- 包含 [nvidia-container-runtime](https://github.com/NVIDIA/nvidia-container-runtime) 和 [nvidia-docker](https://github.com/NVIDIA/nvidia-docker)；
- 实现`runC prestart hook`接口的脚本：该脚本在runC创建一个容器之后，启动该容器之前调用，其主要作用就是修改与容器相关联的config.json，注入一些在容器中使用NVIDIA GPU设备所需要的一些信息（比如：需要挂载哪些GPU设备到容器当中）；
- 将容器runC spec作为输入，然后将`nvidia-container-toolkit`脚本作为一个`prestart hook`注入到runC spec中，将修改后的runC spec交给runC处理。
- CUDA Runtime API和CUDA Libraries通常跟应用程序一起打包到镜像里，而**CUDA Driver API是在宿主机里，需要将其挂载到容器里**才能被使用。

<img src="pics/image-20221122103847948.png" alt="image-20221122103847948" style="zoom: 67%;" />



正常创建一个容器的流程：

> docker --> dockerd --> containerd–> containerd-shim -->runc --> container-process

创建一个使用GPU的容器

> docker–> dockerd --> containerd --> containerd-shim–> nvidia-container-runtime --> nvidia-container-runtime-hook --> libnvidia-container --> runc – > container-process



## K8s 使用

https://github.com/NVIDIA/k8s-device-plugin
