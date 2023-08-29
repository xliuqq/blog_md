# Knative

> [knative doesn't allow you to mount custom volumes](https://github.com/knative/serving/issues/11069#issuecomment-857838668)

Knative 是以 **Kubernetes** 的一组自定义资源类型（CRD）的方式来安装

- 自动完成代码到容器的构建
- 把应用（或者函数）和特定的事件进行绑定：当事件发生时，自动触发应用（或者函数）
- 网络的路由和流量控制
- 函数的自动伸缩

## Build 



## Serving

服务系统，用来配置应用的路由、升级策略、自动扩缩容等功能.



## Eventing

>  *KNative自然也会定义自己的事件类型，除此之外，KNative还联合CNCF在做事件标准化的工作，目前的产出是CloudEvents这个项目。*

事件系统，用来自动完成事件的绑定和触发