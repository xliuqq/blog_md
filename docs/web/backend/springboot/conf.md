# 配置

## 文件加载顺序

SpringBoot 启动会扫描以下位置的 application.yml 或者 application.properties 文件作为SpringBoot的默认配置文件。

```shell
# 优先级由高到低
-file:./config/
-file:./
-classpath:/config/
-classpath:/
```

还可以通过**spring.config.location来改变默认的配置文件位置。**

```shell
java -jar spring-boot-02-config-02.0.0.1-SNAPSHOT.jar  --spring.config.location=G:/application.properties 
```

## 内置Tomcat配置

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

