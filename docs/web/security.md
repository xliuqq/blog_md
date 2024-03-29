# 安全

## 信息安全

### SQL注入

- SQL 拼接；

### XSS

Cross-Site Scripting：跨站脚本攻击，在HTML页面中注入恶意代码。

- **反射型**：诱导用户访问一个精心设计的URL（恶意链接）；
- **存储型**：攻击脚本将被永久地存放在目标服务器端，如发帖时的内容；
- **DOM型**：恶意脚本修改页面的DOM节点

### CSRF

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

      

**Cookie: http-only**

HttpOnly是包含在Set-Cookie HTTP响应头文件中的附加标志，若某一个Cookie 选项被设置成 HttpOnly = true 的话，那此Cookie 只能通过服务器端修改，JS是操作不了的，对于 document.cookie 来说是透明的。



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



## HTTPS

- HTTP + Secure Socket Layer(SSL) 或 Transport Layer Security，在传输层对网络连接进行加密；
- SSL使用证书来创建安全连接；



## 加密解密

- 对称加密：加密和解密密钥相同，如DES和AES；密钥的安全性很重要；
- 非对称加密：公钥和私钥，只能互相加密、解密；如RSA

### AES(Advanced Encryption Standard)

- 分组加密标准，每个加密块大小为128位，允许的密钥长度为128、192和256位；

- ECB：电子密码本模式：Electronic codebook
  - 每块数据的加密独立，因此加密和解密都可以并行计算，最大的缺点是相同的明文块会被加密成相同的密文块，在某些环境不能提供严格的数据保密性；

- CBC模式：密码分组链接：Cipher-block     chaining
  - 每个待加密的密码块在加密前会先与前一个密码块的密文异或然后再用加密器加密，第一个明文块与一个叫初始化向量的数据块异或；

- CFB模式：密文反馈:Cipher feedback
  - 块密文（Block Cipher）转换为流密文（Stream Cipher），适合对流数据进行加密，解密可以并行计算

- OFB模式（输出反馈：Output feedback）
  - 块加密器生成密钥流（Keystream），然后再将密钥流与明文流异或得到密文流，解密是先用块加密器生成密钥流，再将密钥流与密文流异或得到明文；
  - 加密和解密都不能并行

### RSA算法

使用不同的**加密密钥**与**解密密钥**，“由已知加密密钥推导出解密密钥在计算上是不可行的”密码体制。

加密算法E和解密算法D也都是公开的。虽然解密密钥SK是由公开密钥PK决定的，但却不能根据PK计算出SK。

原理：**根据数论，寻求两个大素数比较简单，而将它们的乘积进行因式分解却极其困难，因此可以将乘积公开作为加密密钥**。

步骤：

1. 任意选取两个不同的大素数p和q计算乘积$n=pq$, $\phi(n)=(p-1)(q-1)$；
2. 任意选取一个大整数e，满足$gcd(e,\phi(n)) = 1$，整数e作为加密密钥；
3. 确定解密密钥d，满足$ (de) mod \phi(n) =1$，即$de=k\phi(n)+1,k>=1$，根据$e$和$\phi(n)$很容易计算出d；
4. 公开整数n和e，秘密保存d；
5. 将明文m（m<n是一个整数）加密成密文c，加密算法为$c=E(m)=m^e mod n$
6. 解密密文c，算法为$m=D(c)=c^d mod n$

注：只根据n和e（注意：不是p和q）要计算出d是不可能的

RSA算法本身要求加密内容也就是**明文长度m必须0<m<密钥长度n**:

- 如果小于这个长度就需要进行padding，因为如果没有padding，就无法确定解密后内容的真实长度；
- 如果要对任意长度的数据进行加密，就需要将数据分段后进行逐一加密，并将结果进行拼接。

## 摘要（散列，Hash）算法

- 不同内容的散列值，相同的概率极低；相同内容的散列值一定一样；
- MD5，SHA-1

## 数字签名

- 基于非对称加密，使用私钥进行加密；
- 加密速度慢，使用**摘要算法和非对称算法结合（数字签名）**或**对称加密与非对称加密结合（加密传输文件，对对称密钥加密）**办法；
- 明文+密文，内容被窃取不重要，目的是验证明文的有效正确；



## 数字证书

> 解决信任的问题，防止中间人攻击。

- 数字证书是由一个权威机构发行的，至少包含一个公开密钥、证书持有人（或单位）的名称以及证书授权中心对这些信息的数字签名的文件；
- pfx证书文件是带公钥和私钥，而cer文件只带公钥；
- jks（Java Key Store），jdk/bin/keytool.exe进行查看、导入、导出、删除；
- key通常指私钥；CSR证书签名请求，简单理解成公钥；CRT，CER，证书，编码可以是（PEM格式可读，DER格式二进制不可读）；
- pem文件一般是文本格式的，可以放证书或者私钥，或者两者都有；pem如果只含私钥的话，一般用.key扩展名，而且可以有密码保护；
- 最顶层的权威安全的叫根证书；

### 自签名的证书

- 使用证书生成工具如OpenSSL，jdk的keytool为自己的https站点生成的证书；
- 双向认证：服务端保存着客户端的证书并信任该证书，客户端保存着服务端的证书并信任该证书，如网上银行、企业应用对接；
- 单项认证：客户端保存服务端的证书并信任该证书；





