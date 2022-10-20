# [Baetyl](https://baetyl.io/zh/)

> 前身（OpenEdge）将计算、数据和服务从中心无缝延伸到边缘。

 **[Linux Foundation Edge](https://www.lfedge.org/) 旗下的边缘计算项目，旨在将云计算能力拓展至用户现场**。 

提供**临时离线**、**低延时**的计算服务：

- 设备接入；
- 消息路由；
- 数据遥传；
- 函数计算；
- 视频采集；
- AI推断；
- 状态上报；
- 配置下发

## 架构

![Architecture](pics/baetyl-arch-v2-16662547631395.svg)

### [边缘计算框架（baetyl）](https://github.com/baetyl/baetyl)

边缘计算框架（Edge Computing Framework）运行**在边缘节点的 Kubernetes（K3s） 上**， 管理和部署节点的所有应用

### [云端管理套件（baetyl-cloud）](https://github.com/baetyl/baetyl-cloud/blob/master/README_CN.md)

负责管理所有资源，包括节点、应用、配置、部署等，支持 K8S/K3S 部署，支持单租户和多租户。
