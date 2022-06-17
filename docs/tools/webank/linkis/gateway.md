# 网关

## 路由转发

`/api/rest_[a-zA-Z]+/(v\\d+)/([^/]+)/.+`正则表达式，进行不同服务间的路由匹配：

- 如`/api/rest_j/v1/entrance/`对应`entrance`服务；

- `linkis-ps-publicservice`包含很多子服务：
  - `query,jobhistory,application,configuration,filesystem,udf,variable,microservice,errorcode,bml,datasource`
- 



## 用户和认证

外部的接口，全部经过网关，因此在网关里进行认证。



### 请求拦截

SpringCloudGatewayConfiguration里配置了`GatewayAuthorizationFilter`（实现了SpringCloud的GlobalFilter）

- filter的时候，调用`SecurityFilter`的`doFilter`



### SecurityFilter

> com.webank.wedatasphere.linkis.gateway.security.SecurityFilter

`doFilter`流程（进行安全认证，包括登录、Token等）

- `wds.linkis.enable.gateway.auth`（默认false），为true时，则进行ip白名单校验（`wds.linkis.gateway.auth.file`配置文件）；
- `wds.linkis.server.security.referer.validate`（默认为false），为true时，进行`Referer`域校验（`wds.linkis.server.address`，默认主机名）；
- 无需鉴权的URI，`wds.linkis.gateway.conf.url.pass.auth`默认为`/dss/`；
- 如果uri以`/api/rest_j/v1/user`开头，则进行用户校验并返回消息（**LDAPUserRestful**），返回false；
- **如果uri无需鉴权(以/dss开头)且`wds.linkis.gateway.conf.enable.sso`为false（默认false），则返回true；**
- **如果需要Token鉴权（Header/Cookie中包含`Token-Code`和`Token-User`，则鉴权Token**；
  - `wds.linkis.gateway.conf.enable.token.auth`（默认false），false时直接返回false，输出gateway未采用token认证的消息；
  - `wds.linkis.gateway.conf.token.auth.config`（默认`token.properties`），配置了token-code对应的用户，如果包含`Token-User`则返回true；
- 最后，判断是否已登录，**获取用户信息**（如果`bdp-user-ticket-id`过期，则判断如果Cookie中带`dataworkcloud_inner_request=true`且`bdp-user-ticket-id`解密出来的用户名不是`dataworkcloud_rpc_user`，则返回该用户名；
  - 若未登录（即未获取到用户名），如果开启单点登录（默认false，`wds.linkis.gateway.conf.enable.sso`），则获取用户信息并设置登录用户，如果没有用户信息，若uri无需鉴权，则返回重定向到单点登录地址，如果需要鉴权，则返回单点登录的地址
    - 注：当前没有实现单点登录的接口（配置`wds.linkis.gateway.conf.sso.interceptor`），
- 



#### LDAPUserRestful

`GatewaySpringConfiguration`中配置Spring的默认`UserRestful`的实现，后续可以自定义：



`/api/rest_j/v1/user/XXX` => `XXX`，进行路由处理

- `login`

  1. `Cookie`中`bdp-user-ticket-id`作为登录标识，已登录，则返回；

  2. 获取`用户名/密码/加密密钥`

  3. 管理员用户（即部署用户hadoop/密码和用户名一致，`wds.linkis.admin.user`），则登录成功，且`isAdmin`为true；

  4. 若`wds.linkis.gateway.usercontrol_switch_on`开启（默认false），将登录请求发送到`wds.linkis.usercontrol.application.name`（默认`cloud-usercontrol`，linkis没有该服务），如果返回成功，则创建随机代理用户（`wds.linkis.gateway.conf.proxy.user.list`的配置随机选）作为登录用户

  5. 若`wds.linkis.gateway.redirect_switch_on`开启（默认不开启），向`dss-framework-project-server`工程获取（dss的`ProjectReceiver.scala`）`wds.linkis.gateway.control.workspace.ids`（默认224）下的用户列表（数据库表`dss_workspace_user`），放入到缓存中：

     - 如果登录用户在其中，则走LDAP的login流程；
     - 登录用户不再其中，走另外的gateway(`wds.linkis.gateway.redirect.url`)的进行登录；

     - 数据库数据默认是安装dss用户（hadoop）dss_workspace的id的自增键是224，因此第一个workspace的id就是224；

  6. LDAP login

     ```properties
     wds.linkis.ldap.proxy.url=ldap://127.0.0.1:389/ # 您的LDAP服务URL
     wds.linkis.ldap.proxy.baseDN=dc=webank,dc=com # 您的LDAP服务的配置    
     ```

  7. 登陆成功后，`GatewaySSOUtils.setLoginUser`

     - 若`wds.linkis.gateway.conf.enable.proxy.use`为true（默认false），则将登陆用户转为代理用户；
       - 代理用户在`wds.linkis.gateway.conf.proxy.user.config`（默认`proxy.propetie`）的配置文件里，且定时（默认10分钟）扫描文件更新；
       - 文件格式为   `user=proxyUser`
     - 设置Response的Cookie，需要设置`bdp-user-ticket-id`，将用户和时间戳加密后作为登录标识，放入到内存Map中（标识和过期时间）

  8. 再，有个`securityHooks`的`postLogin`处理；



- `proxy`
  - 设置代理用户，需要`wds.linkis.gateway.conf.enable.proxy.use`等配置；
- `register`（用户注册？）
  - `wds.linkis.gateway.usercontrol_switch_on`开启时，请求对应的微服务（默认`cloud-usercontrol`）；
  - 否则，没有实现，返回错误
- `logout`
- `userInfo`
  - 获取用户名
- `publicKey`
  - 无需认证，获取加密密钥
- `heartbeat`
  - 维持session心跳，每次从cookie中获取用户名的时候，会判断是否session超时（默认2h），如果时间超过session一半时间，则会续期；



#### Token认证
