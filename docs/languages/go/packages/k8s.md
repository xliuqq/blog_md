# K8s

## k8s.io/client-go

k8s 官方的go client



## k8s.io/apimachinery

[apimachinery](https://github.com/kubernetes/kubernetes/tree/master/staging/src/k8s.io/apimachinery)  解决的是 kubernetes 的 API Object 的 Scheme, typing, encoding, decoding, and conversion问题。

This library is **a shared dependency for servers and clients** to work with Kubernetes API infrastructure without direct type dependencies. Its first consumers are `k8s.io/kubernetes`, `k8s.io/client-go`, and `k8s.io/apiserver`.





## [sigs.k8s.io/controller-runtime](https://github.com/kubernetes-sigs/controller-runtime)

> Repo for the controller-runtime subproject of kubebuilder (sig-apimachinery)
>
> a set of go libraries for building Controllers,  leveraged by [Kubebuilder](https://book.kubebuilder.io/) and [Operator SDK](https://github.com/operator-framework/operator-sdk).



## [Kubebuilder](../../../cluster_manager/k8s/crds.md)

一个使用 CRDs（Custom Resource Definition） 构建 K8s API 的 SDK，主要是：

- 提供脚手架工具初始化 CRDs 工程，自动生成 boilerplate 代码和配置；
- 提供代码库封装底层的 K8s go-client；

方便用户从零开始开发 CRDs，Controllers 和 Admission Webhooks 来扩展 K8s。



