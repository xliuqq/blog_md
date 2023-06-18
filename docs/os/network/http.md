# HTTP

HTTP：**H**yper **T**ext **T**ransfer **P**rotocol 

Header

- 空格编码是%20，但是application/x-www-form-urlencoded媒体类型会将空格编码为+号；



## HTTPS

HTTPS：**HTTP** over **S**ecure Socket Layer

- HTTP下加入SSL层，一个加密/身份验证层（在HTTP与TCP之间），端口443；
- SSL(Secure Sockets Layer 安全套接层)及其继任者传输层安全(Transport Layer Security，TLS)是为网络通信提供安全及数据完整性的一种安全协议；
- 采用https的服务器必须从CA（Certificate Authority）申请一个用于证明服务器用途类型的证书。该证书只有用于对应的服务器的时候，客户端才信任此主机。
- HTTPS的性能优化：http://mp.weixin.qq.com/s/V62VYS8KFNKxJxfzMYefrw



## HTTP/1.x

文本协议。

**解析**：HTTP header 各个 fields 使用 `\r\n` 分隔，然后跟 body 之间使用 `\r\n\r\n` 分隔。解析完 header 之后，我们才能从 header 里面的 `content-length` 拿到 body 的 size，从而读取 body。整个流程并不高效。

**交互**：一个连接每次只能一问一答。client需要跟server建立多条连接。

**推送**：HTTP/1.x没有推送机制，一般采用 轮询 或者 Web-socket方式。

## HTTP/2

二进制协议。

- Stream : 一个双向流，一条连接可以有多个streams；
- Message : 等价于request和response；
- Frame : 数据传输的最小单位。每个Frame都属于一个特定的stream或者整个连接。一个message可能由多个frame组成。

### Frame格式

格式如下：

```txt
+-----------------------------------------------+
|                 Length (24)                   |
+---------------+---------------+---------------+
|   Type (8)    |   Flags (8)   |
+-+-------------+---------------+-------------------------------+
|R|                 Stream Identifier (31)                      |
+=+=============================================================+
|                   Frame Payload (0...)                      ...
+---------------------------------------------------------------+
```

Length : Frame的长度，默认最大长度是16KB，可以显示设置max frame size；

Type : Frame类型，如DATA, HEADERS, PRIORITY等；

Flag, R : 保留位；

Stream Identifier : 标志所属的stream，如果为0，则表示这个frame属于整条连接； 

Frame Payload : 不同Type有不同格式。

### 多路复用

HTTP/2的通过**stream支持连接的多路复用**，提高连接利用率。Stream的重要特性：

- 一条连接可以包含多个streams，多个streams发送的数据互相不影响；
- Stream可以被client和server单方面或者共享使用；
- Stream可以被任意一端关闭；
- Stream会确定好发送frame的顺序，另一端会按照接受到的顺序来处理；
- Stream用一个唯一ID来标识，client创建的是奇数，server创建的是偶数。

提高一个stream的并发，可以调大MAX_CONCURRENT_STREAMS。

一条连接通常只有一个线程来处理，所以并不能充分利用服务器多核的优势。同时，**每个请求编解码还是有开销的，所以用一条连接还是会出现瓶颈**。

### 流控

- Flow control 是单向的。Receiver 可以选择给 stream 或者整个连接设置 window size。
- Flow control 是基于信任的。Receiver 只是会给 sender 建议它的初始连接和 stream 的 flow control window size。
- Flow control 不可能被禁止掉。当 HTTP/2 连接建立起来之后，client 和 server 会交换 SETTINGS frames，用来设置 flow control window size。
- Flow control 是 hop-by-hop，并不是 end-to-end 的，也就是我们可以用一个中间人来进行 flow control。

HTTP/2 默认的 window size 是 64 KB。

### HPACK

无需发送重复的Header，提供静态和动态的table。



## Content-Type

### text/event-stream

webpack热更新需要向浏览器推送信息，一般都会想到websocket，但是还有一种方式，叫做**Server-Sent Events**（简称SSE）。

SSE是websocket的一种轻型替代方案。

| sse                      | websocket             |
| ------------------------ | --------------------- |
| http 协议                | 独立的 websocket 协议 |
| 轻量，使用简单           | 相对复杂              |
| 默认支持断线重连         | 需要自己实现断线重连  |
| 文本传输                 | 二进制传输            |
| 支持自定义发送的消息类型 | -                     |



## 错误码

|                      |                                                  |
| -------------------- | ------------------------------------------------ |
| 400(Bad  Request)    | 语法错误无法解读请求                             |
| 401(Unauthorized)    | 无权限访问资源，但身份认证后可以访问权限时       |
| 403(Forbidden)       | 不允许客户端获得资源的访问权限，即使通过身份认证 |
| 404(Not  found)      | 未找到资源                                       |
| 405(Not  Allowed)    | 资源不允许使用某个HTTP方法时                     |
| 406(Not  Acceptable) | 请求不可接受                                     |
| 409(Conflict)        | 请求与资源的当前状态有冲突时                     |
| 410(Gone)            | 资源以前存在，但今后不会存在                     |



## websocket转发

只支持 websocket

```conf
location /websocket/ {
        proxy_pass http://myserver;
        proxy_read_timeout 360s;   
        proxy_redirect off;   
        # 核心配置三行：配置连接为升级连接
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade; 
        proxy_set_header Connection "upgrade";    
        proxy_set_header Host $host:$server_port;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header REMOTE-HOST $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
}
```

支持http又支持 ws（可以见 [jupyterhub 的反向代理配置](https://jupyterhub.readthedocs.io/en/latest/reference/config-proxy.html)）

```conf
	#自定义变量 $connection_upgrade
    map $http_upgrade $connection_upgrade { 
        default          keep-alive;  #默认为keep-alive 可以支持 一般http请求
        'websocket'      upgrade;     #如果为websocket 则为 upgrade 可升级的。
    }
 
    server {
        ...
 
        location /chat/ {
            proxy_pass http://backend;
            proxy_http_version 1.1;
            proxy_set_header Upgrade $http_upgrade; 
            # 此处配置上面定义的变量
            proxy_set_header Connection $connection_upgrade;
        }
    }
```

