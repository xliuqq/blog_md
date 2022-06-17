# CSRF

> CSRF（Cross-site request forgery）跨站请求伪造：攻击者诱导受害者进入第三方网站，在第三方网站中，向被攻击网站发送跨站请求。利用受害者在被攻击网站已经获取的注册凭证，绕过后台的用户验证，达到冒充用户对被攻击的网站执行某项操作的目的。

CSRF的两个特点：

- CSRF（通常）发生在第三方域名。
- CSRF攻击者不能获取到Cookie等信息，只是使用。

针对这两点，可以专门制定防护策略，如下：

- **阻止不明外域的访问**

  - 同源检测：
    - Origin Header：IE11同源策略和302重定向不会存在Origin Header；
    - Referer Header：安全性依赖于浏览器；且可能会不存在
    - 两者都不全备，可能丢失。
  - Samesite Cookie：为Set-Cookie响应头新增Samesite属性
    - Strict：严格模式，表明这个 Cookie 在任何情况下都不可能作为第三方 Cookie；
    - Lax：宽松模式，假如这个请求是这种请求（改变了当前页面或者打开了新页面）且同时是个GET请求，则这个Cookie可以作为第三方Cookie。
    - 问题：子域名之间的单点登录问题？兼容性目前是问题

- **提交时要求附加本域才能获取的信息**

  - **双重Cookie验证**

    - 登陆时向请求域名注册cookie，发起请求时，作为参数传入，后端校验cookie和url中是否一致；
    - 问题：任何跨域都会导致前端无法获取Cookie中的字段（包括子域名之间），所以cookie要设置再根域名下，所有子域名都可以去修改该cookie，导致XSS+CSRF攻击；
    - cookie设置成httponly防止XSS攻击？

  - **CSRF Token**

    - 登陆时，返回csrftoken，后续请求都需要传入该token在Header中；

    - 分布式下，token不存在session中，采用Encrypted Token Pattern，通过UserID，时间戳和随机数，通过加密的方式生成，保证唯一和安全性；

      

Cookie: http-only

HttpOnly是包含在Set-Cookie HTTP响应头文件中的附加标志，若某一个Cookie 选项被设置成 HttpOnly = true 的话，那此Cookie 只能通过服务器端修改，JS是操作不了的，对于 document.cookie 来说是透明的。



## JWT：Json Web Token

**跨域拦截是前端请求已经发出，并且在后端返回响应时检查相关参数，是否允许接收后端请求。**

**通过CORS（跨域资源共享）标准和token来解决资源共享和安全问题。**

举个例子，我们可以设置b.com的响应首部字段为：

> Access-Control-Allow-Origin: http://a.com
>
> Access-Control-Allow-Headers: Authorization, X-Requested-With, Content-Type, Accept
>
> Access-Control-Allow-Methods: GET, POST, PUT,DELETE

第一行指定了**允许访问该资源的外域 URI**

第二行指明了实际请求中**允许携带的首部字段**，这里加入了**Authorization**，用来存放token

第三行用于预检请求的响应。其指明了**实际请求所允许使用的 HTTP 方法**。

然后用户从a.com携带有一个通过了验证的token访问B域名，数据和资源就能够在任何域上被请求到。 