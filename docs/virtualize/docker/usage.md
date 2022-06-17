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
$ docker exec [OPTIONS] Container  Commands [Args]  在正在运行的容器中执行命令
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



## Dockerfile





## Docker in docker

https://blog.csdn.net/networken/article/details/108218569
