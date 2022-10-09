# 内置Tomcat

## 配置

```properties
# 最大工作线程数，默认200。
# 线程数的经验值为：1核2G内存，线程数经验值200；4核8G内存， 线程数经验值800。
server.tomcat.max-threads=200
 
# 最大连接数默认是10000
server.tomcat.max-connections=10000
 
# 等待队列长度，默认100。
server.tomcat.accept-count=100
 
# 最小工作空闲线程数，默认10。
server.tomcat.min-spare-threads=100
```

