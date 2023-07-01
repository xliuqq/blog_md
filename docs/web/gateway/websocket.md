# WebSocket 转发配置

## Nginx 体系

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
        default          keep-alive;  #默认为keep-alive 可以支持一般http请求
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

