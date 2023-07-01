# 会话保持

## Cookie-Session

服务端根据这个Cookie就能找到对应的session，从session中取得用户的信息。

**同源**是指：

- 协议相同：http 或 https
- 域名相同
- 端口相同

**在做前后端分离时，前端和后端部署在同一域下，满足浏览器的同源策略，登录不需要做特殊的处理。**

**前后端不同域时**，需要通过CORS的方式或者JSONP的方式实现Cookie-Session登录。



前后端分离：

- 不同节点部署，会存在跨越访问cookie的问题。	

- sessionId可以通过 response.setHeader("Access-Control-Expose-Headers", "TK") 允许浏览器可以读取哪些服务器发送的请求头

  

示例：华为云

用户中心（ JS + Java）：前后端都存在，起校验（根据Cookie中SessionID去分布式缓存中获取token）、转发（请求转发到后端服务，添加token在Header中）的作用



不需要Session，但还是需要Cookie？

- jwt token之类，还是需要客户端存储



## Token

### Jwt Token

token的续期问题：refresh token + access token

- access token 有效时间短，即使被截获，影响时间范围有限；
- refresh token 不常用，不是所有的请求都会携带，所以更加安全；
- 前端防止并发续期，其它请求等待当前续期的请求返回新的token，再执行；
- 如果不止前端访问，后端可以通过缓存，防止签发多个access token；