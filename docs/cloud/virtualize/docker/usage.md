# 使用

## 部署

```bash
# yum更新
yum update

# 安装必备系统软件
yum install -y yum-utils device-mapper-persistent-data lvm2

# 添加repo
yum-config-manager --add-repo http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo

# 更新yum缓存
yum makecache fast

# 安装docker
yum -y install docker-ce

# 启动 Docker 后台服务
systemctl start docker
systemctl enable docker

# 测试运行 hello-world
docker run hello-world
```



## 源修改

修改或新增 `/etc/docker/daemon.json`

```text
{
"registry-mirrors": "http://hub-mirror.c.163.com"
}
```

然后执行：

```bash
systemctl restart docker.service
```

其中源镜像还可以改为：

Docker 官方中国区：

[https://registry.docker-cn.com](https://link.zhihu.com/?target=https%3A//registry.docker-cn.com)

网易：

[http://hub-mirror.c.163.com](https://link.zhihu.com/?target=http%3A//hub-mirror.c.163.com)

中国科技大学：

[https://docker.mirrors.ustc.edu.cn](https://link.zhihu.com/?target=https%3A//docker.mirrors.ustc.edu.cn)



## 基本命令

### exec

```bash
# 在正在运行的容器中执行命令
$ docker exec [OPTIONS] Container  Commands [Args]
# 交互式
$ docker exec -it [OPTIONS] Container  Commands [Args]  
```

### run

```bash
$ docker run [OPTION] [imageName] [shellCmds]
# 如下，cat /etc/os-release 是执行的CMD，会覆盖DockerFile中的CMD
$ docker run -it ubuntu cat /etc/os-release
$ docker run -d -p 127.0.0.1:5000:5000/udp training/webapp python app.py
```

- **-t** ： 在容器内指定一个伪终端或者终端；  
- **-i** ： 对容器内的标准输入进行交互
- **-d** ： 后台运行，返回唯一的容器ID；     
- **-v**：挂载volume,     [host-dir]:[container-dir]:[rw|ro]
- **-c**：分配容器中所有进程的CPU的share值  
- **-m**：分配容器的内存总量，B、K、M、G单位
- **-P** :是容器内部端口随机映射到主机的高端口；    
- **-p** : 是容器内部端口绑定到指定的主机端口。
- **--rm**：容器运行退出后自动删除
- **--runtime string**：Runtime to use for this container

### images

docker images 来列出本地主机上的镜像

### commit

将正在运行的容器制作新的镜像；

```bash
$ docker commit [OPTIONS] Container [Reopsitory[:TAG]]
```

### build

根据Dockerfile生成镜像

```bash
# 根据当前目录下的Dockerfile生成helloworld镜像
$ docker build -t helloworld .
```

### tag

给容器镜像起完整的名字

```bash
$ docker tag helloworld lzq/helloworld:v1
```

### push

将镜像推送到远程仓库

```
$ docker push lzq/helloworld:v1
```



### rmi

```shell
$ docker rmi f8ab12e03d53
Error response from daemon: conflict: unable to delete f8ab12e03d53 (must be forced) - image is referenced in multiple repositories
```

删除时可以用**repository和tag的方式**来删除

```bash
$ docker rmi 192.168.0.1/you/tom:1.0.8
Untagged:192.168.0.1/you/tom:1.0.8
```



### export/save

`docker export`：将容器的文件系统导出为`tar`，加载为`docker import`

- **容器**持久化（单层Linux文件系统），丢失历史，加载时可以指定镜像名称；
- 适合基础镜像；

`docker save`：镜像导出为 `tar`，加载为`docker load`

- **镜像**持久化（分层文件系统），不丢失历史和层，可通过`docker tag`命令实现层回滚；
- 加载时不可指定镜像名称；





## Dockerfile

`docker build `

### 指令集

```dockerfile
# FROM <image>:<tag>

# MAINTAINER 
 
# ENV <key> <value>

# COPY <src> <dst>
# <src> 支持通配符和多个源

# ADD <src> <dst>
# <src> 对于本地压缩归档文件，复制到容器中会被解压提取，推荐使用 COPY

# RUN <command> (shell格式)
# RUN  ["executable", "param1", "param2"] （exec格式，推荐）
# 在前一个命令创建出的镜像基础上创建一个容器，在容器中运行命令，命令结束后提交容器为新镜像

# CMD <command> (shell格式)
# CMD  ["executable", "param1", "param2"] （exec格式，推荐）
# 容器运行时的默认值，配合ENTRYPOINT使用，会被docker run命令时指定的命令参数覆盖

# ENTRYPOINT <command> (shell格式)
# ENTRYPOINT  ["executable", "param1", "param2"] （exec格式，推荐）
# shell格式时，会忽略 CMD 和 docker run的命令的参数，运行在 /bin.sh -c 中，非1号进程

# ONBUILD [INSTRUCTION]
# 添加一个将来执行的触发器指令到镜像中。

# VOLUME

# USER

# WORKDIR

# EXPOSE
```



### 实践

RUN：

- `RUN apt-get update && apt-get install -y foo`应该放一起，避免缓存问题；

- 不要将所有的命令写在一个RUN指令中；

Expose：

- 不要在Dockerfile中做端口映射，缺乏可移植性；



## Docker in docker

https://blog.csdn.net/networken/article/details/108218569



## Alpine

### Alpine-docker容器中安装GCC

```shell
RUN sed -i 's/dl-cdn.alpinelinux.org/mirrors.ustc.edu.cn/g' /etc/apk/repositories

RUN apt add --update  build-base && rm -rf /var/cache/apk/*

```

`build-base` is a meta-package that will install the GCC, libc-dev and binutils packages (amongst others).
