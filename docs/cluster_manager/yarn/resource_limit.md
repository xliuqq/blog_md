[toc]

# 资源限制

## 默认

yarn container 默认不支持对cpu进行资源隔离，例如申请的1个vcore，实际上又启动了多线程，还有GC线程等都会造成资源使用不可控。

内存限制，通过 Java 启动指定-Xmx，也不是Container真实的内存限制。

## Cgroup

默认情况下，NodeManager 使用 `DefaultContainerExecutor` 以 **NodeManager 启动者的身份来执行启动Container**等操作，安全性低且没有任何CPU资源隔离机制。

要使用 `LinuxContainerExecutor`，从而以应用提交者的身份创建文件，运行/销毁 Container。允许**用户在启动Container后直接将CPU份额和进程ID写入cgroup路径的方式实现CPU资源隔离**。

Cgroup 是linux kernel的一个功能，可以资源进行隔离，Yarn中现在支持对**cpu/mem/io** 和 **gpu/fpage**等资源进行隔离。

- **cpushares**隔离: 按权重比率弹性分配cpu时间；
  - 当cpu空闲的时候，某一个要占用cpu的cgroup可以完全占用剩余cpu时间，充分利用资源。而当其他cgroup需要占用的时候，每个cgroup都能保证其最低占用时间比率，达到资源隔离的效果。
- **cpuset**隔离: 以分配核心的方式进行资源隔离；
  - 。/资源分配最小粒度是核，不能提供更细粒度的资源隔离，但是隔离之后运算的相互影响最低。需要注意的是在服务器开启了超线程的情况下，要小心选择分配的核心，否则不同cgroup间的性能差距会比较大。
- **cpuquota**隔离： 给我们提供了一种比cpuset可以更细粒度的分配资源的方式，并且保证了cgroup使用cpu比率的上限，相当于对cpu资源的硬限制。



gpu/fpga等其它countable资源的隔离，采用插件机制，定义资源发现/分配/隔离的实现类，通过cgroup的`devices`进行隔离。