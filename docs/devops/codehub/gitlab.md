# Gitlab 



## 安装

### 官方docker 安装

> 将 gitlab, nginx, postgres, redis 都运行在一个镜像中

* 拉取gitlab-ce镜像
```shell
docker pull gitlab/gitlab-ce
```
* 将 GitLab 的配置 (etc) 、 日志 (log) 、数据 (data) 放到容器之外， 便于日后升级， 因此请先准备这三个目录。
```shell
mkdir -p /mnt/gitlab/etc
mkdir -p /mnt/gitlab/log
mkdir -p /mnt/gitlab/data
```
* 启动镜像，需要建立端口映射，8090和22是容器内gitlab的http和ssh的端口
``` shell
docker run \
    --detach \
    --publish 8090:8090 \
    --publish 222:22 \
    --name gitlab \
    --restart unless-stopped \
    -v /mnt/gitlab/etc:/etc/gitlab \
    -v /mnt/gitlab/log:/var/log/gitlab \
    -v /mnt/gitlab/data:/var/opt/gitlab \
    gitlab/gitlab-ce
```
* 配置gitlab
gitlab上创建项目的时候，生成项目的URL访问地址是按容器的hostname来生成的，也就是容器的id。作为gitlab服务器，我们需要一个固定的URL访问地址，于是需要配置gitlab.rb（宿主机路径：/mnt/gitlab/etc/gitlab.rb）
```shell
# gitlab.rb文件内容默认全是注释
vim /mnt/gitlab/etc/gitlab.rb
```
```shell
# 配置http协议所使用的访问地址,不加端口号默认为80
external_url 'https://192.168.199.231:8090'
# 配置ssh协议所使用的访问地址和端口
gitlab_rails['gitlab_ssh_host'] = '192.168.199.231'
gitlab_rails['gitlab_shell_ssh_port'] = 222 # 此端口是run时22端口映射的222端口

#保存配置文件并退出
:wq
```
* 重启gitlab容器

  `docker restart gitlab`

### FAQ

#### GitLab 访问返回 502

gitlab-ctl status 查看对应的服务，是否有不停重启的服务，进而查看服务日志。

unicorn一直重试，

- 端口问题，改unicorn端口，再对gitlab重启；
- 没有明显原因，则排查资源问题（CPU和内存）；
- 尝试 docker exec -it gitlab rm /opt/gitlab/var/unicorn/unicorn.pid && docker restart gitlab

https://forum.gitlab.com/t/error-502-failed-to-start-a-new-unicorn-master/29790

### 三方docker安装

https://github.com/sameersbn/docker-gitlab

拆分为 gitlab, postgres, redis 三个镜像，通过 docker compose 启动；



## 操作命令

docker容器安装gitlab时，需要先进入到容器中

 `docker exec -ti gitlab /bin/bash`

* 重新应用gitlab的配置    

  `gitlab-ctl reconfigure`

* 重启gitlab服务    

  `gitlab-ctl restart`

* 查看gitlab运行状态    

  `gitlab-ctl status`

* 停止gitlab服务    

  `gitlab-ctl stop`

* 查看gitlab运行日志    

  `gitlab-ctl tail`

* 停止相关数据连接服务   

  `gitlab-ctl stop unicorn  `  

  `gitlab-ctl stop sideki`
