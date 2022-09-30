[toc]

# Yarn Application

## 接口类

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

## Client

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