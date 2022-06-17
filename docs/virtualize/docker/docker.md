[toc]

# Docker

同一台机器上的所有容器共享主机操作系统的内核。

- 如果应用程序需要配置内核参数、加载额外内核模块、与内核直接交互，是全局可见的。



## 基础知识

- 镜像是多层存储，每一层的东西并不会在下一层被删除，会一直跟随着镜像；
- 命令行工具docker与Docker daemon建立通信，Docker daemon是Docker守护进程，负责接收并分发执行Docker命令；
- docker命令执行需要获取root权限，绑定属于root的Unix Socket；



## 镜像

### 联合挂载

镜像设计过程中引入了层（layer）的概念，制作镜像的每一步都会生成一个层，形成增量的rootfs；

- UnionFS：将**不同位置的目录联合挂载（union mount）到同一个目录**下；

- 通过`docker info`查看底层`Storage Driver`，centos上为`overlay2`；

#### 只读层

readonly + whiteout

#### 读写层

readwrite层，删除foo文件会通过新增`.wh.foo`文件，进行"遮挡"。

docker commit和push指令保存修改过的可读写层，形成新的镜像，共别人使用。

#### Init层

Init层是一个以`-init`结尾的层，夹在只读层和可读写层之间，用于存放`/etc/hosts`、`/etc/resolv.conf`等信息；

- 这些文件本来属于Ubuntu镜像的一部分，但是用户会在启动容器时修改他们，但是这些修改不具备普适性，不应该被commit。



docker中使用overlay2存储的原理：https://zhuanlan.zhihu.com/p/41958018



## 隔离

基于Linux中的[`Namespace`机制](../../linux/namespace.md)。

### 用户（User Namespace）

> 本节掌握**用户名、组名、用户 id(uid)和组 id(gid)**如何在**容器内的进程和主机系统之间映射**。

***docker 默认并没有启用 user namesapce***。

默认情况下，容器中的进程以 root 用户权限运行，并且这个 **root 用户和宿主机中的 root 是同一个用户**。

> **内核使用的是 uid 和 gid，而不是用户名和组名**，判断对文件的访问权限。

- **内核控制的 uid 和 gid 则仍然只有一套**，**同一个 uid 在宿主机和容器中代表的是同一个用户**（即便在不同的地方显示了不同的用户名）；

通过 docker run 命令的 --user 参数指定容器中进程的用户身份；

通过 Dockerfile 中的**`RUN useradd -r -u 1000 -g appuser; USER appuser` **添加和指定容器内的用户；

#### 配置

创建 /etc/docker/daemon.json 文件：

```
$ sudo touch /etc/docker/daemon.json
```

然后编辑其内容如下(如果该文件已经存在，仅添加下面的配置项即可)，并重启 docker 服务：

```json
{
  "userns-remap": "default"
}
```

```bash
# docker 会创建了一个名为 dockremap 的用户
# /etc/subuid 和 /etc/subgid 文件中会添加了新用户 dockremap
# /var/lib/docker 目录下会新建了对应的dockremap的目录
$ sudo systemctl restart docker.service
```



## 资源限制

基于[`cgroup`](../../linux/cgroup.md)限制



## 网络

docker 提供给我们多种(4种)网络模式：

- 默认网络模式 ： bridge；
- 无网络模式 ：none；
- 宿主网络模式 ：host；
- 自定义网络：允许创建三种类型的自定义网络，bridge，overlay，MACVLAN

### overlay

实现docker 容器跨主机互通。 推荐使用 overlay 网络类型。

swarm在设计之初是为了service(一组container)而服务的，因此通过swarm创建的overlay网络在一开始并不支持单独的container加入其中。但是在docker1.13, 我们可以通过**“--attach” 参数声明当前创建的overlay网络可以被container直接加入**。

```shell
$ docker network create --driver=overlay --attachable name=myOverlayNe
```

swarm创建overlay网络：

```shell
# Create swarm in one docker host
$ docker swarm init –advertise-address 172.100.1.17
# Wait for worker node joined
$ docker swarm join-token worker
# docker swarm join-token worker
$ docker swarm join --token SWMTKN-1-4kaj1vanh45ihjfgud7nfgaj099gtvrgssg4dxp4rikd1kt1p1-6bwep9vx83oppouz0rfz5scf9 172.100.1.17:2377
# Create network on manager node --attachable 是关键，它表明这个网络是可以被container所加入。
$ docker network create -d overlay --attachable qrtOverlayNet


```



## 日志

Docker 容器引擎将 stdout/stderr 输出流重定向到某个 [日志驱动（Logging Driver）](https://docs.docker.com/engine/admin/logging/overview) ， 以 JSON 格式（默认）写入文件（***/var/lib/docker/containers/<容器id>/<容器id>-json.log***，且不会限制大小）。
