# Yarn Application

## 原生

### 接口类

- **Client ** <--> **ResourceManager**

   `YarnClient` 

- **ApplicationMaster**<-->**ResourceManager**

  通过 `AMRMClientAsync` 对象，通过 `AMRMClientAsync.CallbackHandler` 异步处理事件

- **ApplicationMaster**<-->**NodeManager**

  启动Container，通过 `NMClientAsync` 和NodeManager通信，通过 `NMClientAsync.CallbackHandler`处理container事件

  

**更底层的API，一般不推荐使用**

ApplicationClientProtocol ：**Client ** <--> **ResourceManager**

ApplicationMasterProtocol ：**ApplicationMaster**<-->**ResourceManager**

ContainerManagementProtocol ：**ApplicationMaster**<-->**NodeManager**

### Client

- **启动YarnClient**

```java
YarnClient yarnClient = YarnClient.createYarnClient();
yarnClient.init(conf);
yarnClient.start();
```

- **创建应用，获取应用id**

```java
YarnClientApplication app = yarnClient.createApplication();
GetNewApplicationResponse appResponse = app.getNewApplicationResponse();
```

### TODO



## 第三方[skein](https://github.com/jcrist/skein)

> A tool(cli) and library (python) for easily deploying applications on Apache YARN.



### 原理

Java 写的 Driver 服务，作为 Yarn Client跟Yarn RM通信；

Java 写的 Application Master 服务：

- 负责解析Skin App Yaml，向Yarn申请/释放Container，同时可以运行一个用户定义的进程（application driver）；
- 包含 [KV Store](https://jcristharif.com/skein/key-value-store.html)，可用于container间信息共享或者协调状态等；

### 规范

支持 Master 和 Service；

### 使用

1. 安装

```shell
$ conda install -c conda-forge skein
```

2. 启动 driver，全局driver，如果不启动全局，则每次提交时会启动临时的driver；

```shell
$ skein driver start
```

3. 写应用程序定义

```yaml
name: hello_world
queue: default

master:
  resources:
    vcores: 1
    memory: 512 MiB
  script: |
    sleep 60
    echo "Hello World!"
```

4. 提交程序

```shell
$ skein application submit hello_world.yaml
```

