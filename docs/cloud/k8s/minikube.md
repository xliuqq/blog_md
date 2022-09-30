# MiniKube



## 安装

### Windows

https://minikube.sigs.k8s.io/docs/start/

Windows配置Hyper -V

安装 Hyper-V

> 【控制面板】->【程序】->【打开和关闭 windows 功能】-> 勾选 Hyper-V 的选项（包括所有子项）-> 重启电脑

启用 Hyper-V

```powershell
Enable-WindowsOptionalFeature -Online -FeatureName Microsoft-Hyper-V -All
```



```shell
minikube start --driver=hyperv 
# To make hyperv the default driver:
minikube config set driver hyperv
```



```bash
minikube start --disk-size="10g"  --image-mirror-country="cn"  --image-repository="registry.cn-hangzhou.aliyuncs.com/google_containers"
```