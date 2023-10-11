# K8s

> 参考：
>
> 1. [k8s-club/articles/Informer机制 - 概述](https://github.com/k8s-club/k8s-club/blob/main/articles/Informer机制 - 概述.md#k8s-informer机制概述)



## k8s.io/client-go

k8s 官方的go client



### List-Watch机制

> Etcd存储集群的数据信息，apiserver作为统一入口，任何对数据的操作都必须经过apiserver。
>
> - api server 对外提供 HTTP 服务；
> - 客户端（kubelet/scheduler/controller-manager）通过`list-watch`监听资源的`create, update, delete`事件；

List：全量数据

Watch：增量数据，同时存在 HTTP 1.1 和 Websocket 的实现

- Http Transfer-Encoding: chunked，通过 `curl` 进行 watch；

如何保证同一个资源的变化事件的顺序性？

- TODO：基于 etcd 的 watch 机制，由 etcd 保证？



### Informer

> `Informer`被设计实现为一个依赖`Kubernetes List/Watch API`、`可监听事件并触发回调函数`的`二级缓存`工具包。
>
> - 更快地返回`List/Get`请求的结果、减少对`Kubenetes API`的直接调用

Informer`实例的`Lister()`方法，`List/Get Kubernetes`中的`Object`时，`Informer`不会去请求`Kubernetes API，直接查找`缓存`在本地内存中的数据。

封装`list-watch API`，用户只需要指定资源，编写事件处理函数，`AddFunc`,`UpdateFunc`和`DeleteFunc`等。

- 针对 Update 场景，可以拿到 old 和 new 的对象；
- list-watch 如果出错后，会再次调用 list-watch；

二级缓存`DeltaFIFO`和`LocalStore`

- `DeltaFIFO`用来存储`Watch API`返回的各种事件 
- `LocalStore`只会被`Lister`的`List/Get`方法访问

`Informer`和`Kubernetes`之间没有`resync`机制，但`Informer`内部的这两级缓存之间存在`resync`机制。

- `resync`将 `LocalStore` 中的对象，全部发送到 `DeltaFIFO` 的队列中；
- 触发的是 update 事件，其`（old, new）`对象一致；



### Reflector



### Indexer



## k8s.io/apimachinery

[apimachinery](https://github.com/kubernetes/apimachinery)  解决的是 kubernetes 的 API Object 的 Scheme, typing, encoding, decoding, and conversion问题。

This library is **a shared dependency for servers and clients** to work with Kubernetes API infrastructure without direct type dependencies. Its first consumers are `k8s.io/kubernetes`, `k8s.io/client-go`, and `k8s.io/apiserver`.





## [Kubebuilder](../../../cloud/k8s/crds_kubebuild.md)

一个使用 CRDs（Custom Resource Definition） 构建 K8s API 的 SDK，主要是：

- 提供脚手架工具初始化 CRDs 工程，自动生成 boilerplate 代码和配置；
- 提供代码库封装底层的 K8s go-client；

方便用户从零开始开发 CRDs，Controllers 和 Admission Webhooks 来扩展 K8s。



### [sigs.k8s.io/controller-runtime](https://github.com/kubernetes-sigs/controller-runtime)

> Repo for the controller-runtime subproject of kubebuilder (sig-apimachinery)
>
> a set of go libraries for building Controllers,  leveraged by [Kubebuilder](https://book.kubebuilder.io/) and [Operator SDK](https://github.com/operator-framework/operator-sdk).

