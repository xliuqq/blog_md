# YARN Pluggable Device Framework

 插件式的适配支持：

- 依赖 `LinuxContainerExecutor ` 进行 Cgroup 隔离

## 配置

`capacity-scheduler.xml`

```xml
<property>
  <name>yarn.scheduler.capacity.resource-calculator</name>
  <value>org.apache.hadoop.yarn.util.resource.DominantResourceCalculator</value>
</property>
```

`yarn-site.xml`：开启设备插件框架

```xml
<property>
  <name>yarn.nodemanager.pluggable-device-framework.enabled</name>
  <value>true</value>
</property>
```

自定义设备资源和处理的类：

```xml
<property>
  <name>yarn.resource-types</name>
  <value>nvidia.com/gpu</value>
</property>
<property>
  <name>yarn.nodemanager.pluggable-device-framework.device-classes</name> 		     <value>org.apache.hadoop.yarn.server.nodemanager.containermanager.resourceplugin.com.nvidia.NvidiaGPUPluginForRuntimeV2</value>
</property>
```

通过`yarn node -list -showDetails`可以查看设备资源信息。



## 开发插件

### 插件原理



### Demo

maven 依赖

```xml
<dependencies>
  <dependency>
    <groupId>org.apache.hadoop</groupId>
      <artifactId>hadoop-yarn-server-nodemanager</artifactId>
      <version>3.3.0</version>
      <scope>provided</scope>
  </dependency>
</dependencies>
```