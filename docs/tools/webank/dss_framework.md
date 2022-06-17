# DSS框架

**DSS插拔式的框架设计模式，允许用户快速替换DSS已集成的各个Web系统**。如：将 Scriptis 替换成Zeppelin，将 Schedulis 替换成DolphinScheduler。



## AppConn

 AppConn——应用连接器，定义了一套统一的前后台接入协议，总共分为三级规范，可让外部数据应用系统快速简单地接入，成为DSS数据应用开发中的一环。

AppConn的三级规范即：**一级SSO规范**，**二级组织结构规范**，**三级开发流程规范**；

DSS通过串联多个 AppConn，编排成一条支持**实时执行和定时调度**的工作流，用户只需简单拖拽即可完成数据应用的全流程开发。

由于 AppConn 对接了Linkis，外部数据应用系统因此具备了**资源管控、并发限流、用户资源管理**等能力，且允许**上下文信息跨系统级共享**，彻底告别应用孤岛。



dss工程的`appconn_install.sh`：

1. 指定插件服务的 IP 和 Port，这里一般写Nginx的IP和端口号（即使是本机，也不要写127.0.0.1）；
2. 执行刷库脚本；
3. 在`dss-appconns/${APPCONN_NAME}/db`，将`init.sql`进行变量替换，形成最终的刷库脚本，通过`mysql`执行刷库；
   - APPCONN_INSTALL_IP 、APPCONN_INSTALL_PORT、DSS_INSTALL_HOME_VAL
   - 数据库表：
     - dss_application
     - dss_menu
     - dss_onestop_menu_application
     - dss_appconn
     - dss_appconn_instance
     - dss_workflow_node
     - dss_workflow_node_to_group
     - dss_workflow_node_to_ui
     - 
4. 调用`http://${GATEWAY_INSTALL_IP}:${GATEWAY_PORT}/api/rest_j/v1/dss/framework/project/appconn/${APPCONN_NAME}/load`通知加载服务；
   - dss-framework-project-server  =>  linkis gateway => ?



数据库解耦（刷的都是dss的库）：

- 刷库脚本 => Restful 调用





### 用户

### Linkis

linkis 的用户可以通过**插件形式**提供认证服务，需要实现 login接口

```json
{
    "method": null,
    "status": 0,
    "message": "login successful(登录成功)！",
    "data": {
        "isAdmin": false,
        "userName": ""
    }
}
```

- isAdmin: Linkis只有admin用户和非admin用户，admin用户的唯一特权，就是支持在Linkis管理台查看所有用户的历史任务。



## 项目

linkis 不支持单个项目级别的配置，比如队列信息，spark等配置信息。



Java/Scala API 支持指定队列，没有真实使用！



label的概念？