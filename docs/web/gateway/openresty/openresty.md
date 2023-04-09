

# OpenResty

是一款基于 NGINX 和 LuaJIT 的 Web 平台，提供[丰富的插件](http://openresty.org/cn/components.html)。

## 安装

### 源码安装

参考官网 http://openresty.org/cn/installation.html

**依赖**

```bash
yum install pcre-devel openssl-devel gcc curl
```

**编译**

```
./configure --prefix=/opt/openresty \
--with-luajit \
--with-http_iconv_module 
```

--with-http_postgres_module 需要安装 libpq

**安装**

```
make -j4
make install
```



## 配置

### nginx.conf

### 日志清理



### websocket转发

```ini
proxy_set_header Upgrade $http_upgrade;
proxy_set_header Connection "upgrade";
proxy_http_version 1.1;
```

