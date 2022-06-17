# Apache Shiro

开源安全框架，处理身份验证，授权，企业会话管理和加密。

![shiro arch](pics/ShiroArchitecture.png)



创建`SecurityManager`安全管理器 -> 主体`Subject`提交认证信息 -> `SecurityManager`安全管理器认证 ->  `SecurityManager`调用`Authenticator`认证器认证 ->`Realm`验证

**Subject**

主体，代表了当前“用户”；所有`Subject`都绑定到`SecurityManager`，与`Subject`的所有交互都会委托给`SecurityManager`；

**SecurityManager**

所有与安全有关的操作都会与`SecurityManager`交互；且它管理着所有`Subject`；负责与后边介绍的其他组件进行交互。

**Realms**

域，Shiro从从`Realm`获取安全数据（如用户、角色、权限），就是说`SecurityManager`要验证用户身份，那么它需要从`Realm`获取相应的用户进行比较以确定用户身份是否合法；也需要从`Realm`得到用户相应的角色/权限进行验证用户是否能进行操作；可以把Realm看成`DataSource`，即安全数据源。



### 内置的过滤器

authc：所有已登陆用户可访问

roles：有指定角色的用户可访问，通过[ ]指定具体角色，这里的角色名称与数据库中配置一致

perms：有指定权限的用户可访问，通过[ ]指定具体权限，这里的权限名称与数据库中配置一致

anon：所有用户可访问，通常作为指定页面的静态资源时使用