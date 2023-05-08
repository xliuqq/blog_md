# 代理用户

`core-site/xml`：

```xml
 <property>
     <name>hadoop.proxyuser.super.hosts</name>
     <value>host1,host2</value>
   </property>
   <property>
     <!-- super 用户可以模拟group1, group2下的用户 -->
     <name>hadoop.proxyuser.super.groups</name>
     <value>group1,group2</value>
   </property>
 <property>
     <!-- super 用户可以模拟user1, user2用户 -->
     <name>hadoop.proxyuser.super.users</name>
     <value>user1,user2</value>
   </property>
```



```java
// 如果启动进程的用户不是配置的超级用户，则可以通过环境变量 HADOOP_PROXY_USER 指定为相应的超级用户即可
UserGroupInformation ugi =
                UserGroupInformation.createProxyUser(userName, UserGroupInformation.getLoginUser());
return ugi.doAs(action);
```



## Kerberos

```java
// 设置全局的登录用户
UserGroupInformation.loginUserFromKeytab("hq/master@HQGF.COM","E:/kerberosconf/hq.keytab");
// 代理用户
UserGroupInformation ugi = UserGroupInformation.createProxyUser(userName, UserGroupInformation.getLoginUser());
ugi.doAs(action);
```



```java
// 设置当前用户
UserGroupInformation ugi = UserGroupInformation.loginUserFromKeytabAndReturnUGI("hq/master@HQGF.COM","E:/kerberosconf/hq.keytab");
ugi.doAs(action);
```

