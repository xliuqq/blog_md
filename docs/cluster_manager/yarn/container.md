# 容器（Container）



## 类型

### DefaultContainerExecutor

- 默认的executor，管理容器的执行；
- 容器进程的用户是NodeManager的Unix User；



### LinuxContainerExecutor

容器进程的用户：

- 开启 full security 时，是提交应用的Yarn User（因此需要在对应的NodeManager中，该用户存在，多租户下可能会有问题）；
- 未开启 full security时，默认为 nobody用户，通过配置可以修改：
  - `yarn.nodemanager.linux-container-executor.nonsecure-mode.limit-users`：是否在非安全模式下，限制运行用户为单个用户；
  - `yarn.nodemanager.linux-container-executor.nonsecure-mode.local-user`：非安全模式下的运行用户；

| yarn.nodemanager.linux-container-executor.nonsecure-mode.local-user | yarn.nodemanager.linux-container-executor.nonsecure-mode.limit-users | User running jobs         |
| :----------------------------------------------------------- | :----------------------------------------------------------- | :------------------------ |
| (default：nobody)                                            | (default：true)                                              | nobody                    |
| yarn                                                         | (default：true)                                              | yarn                      |
| yarn                                                         | false                                                        | (User submitting the job) |

