# K8s调度器

## Kubernetes Scheduling Framework（TODO）

> https://mp.weixin.qq.com/s/UkVXuZU0E0LT3LaDdZG4Xg
>
> https://zhuanlan.zhihu.com/p/159736779

在执行调度流程时，运行到相应的扩展点时会调用用户注册的插件，影响调度决策的结果。将用户的调度逻辑集成到 Scheduling Framework 中。

分为两个阶段 scheduling cycle 和 binding cycle。

- scheduling cycle 是同步执行的，同一个时间只有一个 scheduling cycle，是线程安全的；
- binding cycle 是异步执行的，同一个时间中可能会有多个 binding cycle在运行，是线程不安全的。