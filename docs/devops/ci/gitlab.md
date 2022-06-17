# Gitlab



## 安装（docker）

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
:wq #保存配置文件并退出
```
* 重启gitlab容器

  docker restart gitlab

### 问题定位指南

#### GitLab 访问返回 502

gitlab-ctl status 查看对应的服务，是否有不停重启的服务，进而查看服务日志。

unicorn一直重试，

- 端口问题，改unicorn端口，再对gitlab重启；
- 没有明显原因，则排查资源问题（CPU和内存）；
- 尝试 docker exec -it gitlab rm /opt/gitlab/var/unicorn/unicorn.pid && docker restart gitlab

https://forum.gitlab.com/t/error-502-failed-to-start-a-new-unicorn-master/29790



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



## CI/CD

### Runner

#### Docker-Runner

Gitlab-runner 安装：更好的管理方式是k8s

* 拉取镜像

```shell
docker pull gitlab/gitlab-runner
```

* 启动gitlab-runner

```shell
docker run -d --name gitlab-runner --restart always \
    -v /mnt/gitlab-runner/config:/etc/gitlab-runner \
    -v /var/run/docker.sock:/var/run/docker.sock \
    gitlab/gitlab-runner
```

* 注册到gitlab 
  根据gitlab admin中的runner的信息，填写以下信息

```shell
docker run --rm -t -i -v /mnt/gitlab-runner/config:/etc/gitlab-runner gitlab/gitlab-runner register \
  --non-interactive \
  --url "http://172.16.1.181:8090/" \
  --registration-token "DA1wNdAchnBFH_frXa9N" \
  --executor "docker" \
  --docker-image alpine:latest \
  --description "docker-runner" \
  --tag-list "docker,test"
```

如果出现了no route to host异常，需要在宿主机上添加端口防火墙(原因见docker章节**No Route to Host** 问题)

```shell
firewall-cmd --zone=public --add-port=8090/tcp --permanent
firewall-cmd --reload
```

#### K8s-Runner

Yaml配置原件：[gitlab-runner.yaml](./materials/gitlab-runner.yaml)，注意修改gitlab地址和runner token。

注意:

- replicas: Runner实例数

- CI_SERVER_URL: gitlab服务器地址
- GITLAB_CI_TOKEN: gitlab项目token
- token需要base转换: echo oqdKGRBZ-FzcVp9iyvwP | base64 -w0

创建runner：`kubectl create -f gitlab-runner.yaml`



### 代码工程配置

需要在项目中创建 `.gitlab-ci.yml` 文件，下面是个示例，其中tags是创建gitlab-runner时指定的tags，匹配上才会有runner执行CI：

```yaml
image: maven:latest
stages:
  - build
  - test
  - run
variables:
  MAVEN_OPTS: "-Dmaven.repo.local=.m2/repository"
cache:
  paths:
    - .m2/repository/
    - target/
build:
  stage: build
  script:
    - mvn $MAVEN_CLI_OPTS compile
  only:
    - master
  tags:
    - test
test:
  stage: test
  script:
    - mvn $MAVEN_CLI_OPTS test
  only:
    - master
  tags:
    - test
deploy:
  stage: deploy
  script:
    - echo "deploy over..."
  only:
    - master
  tags:
    - test
```

### cache

cache是用来指定 **jobs** 之间可以缓存的文件和目录

Locally defined cache overrides globally defined options. 

#### maven项目配置缓存

```yaml
image: nnntln/3.6.1-jdk-8:latest

variables:
   MAVEN_OPTS: -Dmaven.repo.local=/cache/maven.repository
cache:
   key: PortalReportBackend
   paths:
     - /root/.m2/repository

stages:
  - build
  - execute

build:
  stage: build
  script: /usr/lib/jvm/java-8-openjdk-amd64/bin/javac Hello.java
  artifacts:
    paths:
     - Hello.*

execute:
  stage: execute
  script: /usr/lib/jvm/java-8-openjdk-amd64/bin/java Hello
```

### artifacts 

`artifacts` is used to specify **a list of files and directories which should be attached to the job** when it succeeds, fails, or always.

The artifacts will be sent to GitLab after the job finishes and will be available for download in the GitLab UI.

