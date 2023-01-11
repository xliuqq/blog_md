# Gitlab CI

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

