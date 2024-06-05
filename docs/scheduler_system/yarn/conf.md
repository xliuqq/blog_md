# 配置

## 基本配置



## GPU基本配置

见 [GPU](./gpu.md)

## yarn

### yarn.scheduler.capacity.resource-calculator

`org.apache.hadoop.yarn.util.resource.DefaultResourceCalculator`：默认

`org.apache.hadoop.yarn.util.resource.DominantResourceCalculator`：一般需要改成这个，web ui显示的和实际申请的会一致。



## 日志聚合

`yarn-site.xml`

- 远程的日志路径是 `{yarn.nodemanager.remote-app-log-dir}/${user}/{yarn.nodemanager.remote-app-log-dir-suffix}`

```xml
<property>
    <!-- 下面路径是默认值，不需要配置-->
	<name>yarn.log.server.url</name>
	<value>http://node01:19888/jobhistory/logs</value>
</property>

<property>
    <!-- 默认不开启 -->
    <name>yarn.log-aggregation-enable</name>  
    <value>true</value>  
</property>  
<property>  
    <!-- 设置日志保留时间，单位是秒，默认 -1，不会删除聚合后的日志 -->
    <name>yarn.log-aggregation.retain-seconds</name>  
    <value>640800</value>  
</property>
<property>  
    <!-- HDFS中的一个目录，默认是 /tmp/logs -->
    <name>yarn.nodemanager.remote-app-log-dir</name>  
    <value>/hdfs/path/to/logs</value>  
</property>
<property>
    <!-- moren logs, 为每个应用程序的日志目录添加一个后缀。可以更好地组织和管理日志 -->
    <name>yarn.nodemanager.remote-app-log-dir-suffix</name>  
    <value>logs-${user.name}</value>  
</property>
```

