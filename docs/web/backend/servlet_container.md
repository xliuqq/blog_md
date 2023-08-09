# Servlet 容器

> HTTP 服务

常见的Servlet 容器有：Tomcat，Jetty，Undertow等。

Tomcat负责请求连接，断开连接，连接校验等操作，Servlet主要处理内部数据操作。

- Tomcat 可以理解成用于 http平台的服务器，能接受http的请求并分析；
- Servlet是java内部的一种规范，准确的说，可以对参数进行分析，创建对象然后返回数据。



## 常见的网络容器

- **Servlet** ：Java Servlet和 Java Server Page，动态生成网页；
  - 不是线程安全，servlet不是线程，通过线程池运行servlet中的处理请求的方法；

- **Tomcat**、**Jetty** ：**servlet引擎**，支持标准的servlet规范和javaEE规范；认为**应用服务器**，搭配具体的运行环境；
  - Jetty：基于Handler设计，易扩展，按需加载组件；HTTP客户端是非阻塞和异步的；
  - Tomcat：基于容量设计，不易扩展；

- **Apache**、**Nginx** ：HTTP 服务器软件
  - 静态资源访问，可以通过脚本方式进行扩展；
  - Tomcat也可以认为是HTTP服务器，但通常它仍然会和Apache或Nginx配合在一起使用，用于动态、静态资源分离以及负载均衡；



**NIO、Netty、Grizzly**

- NIO：Network IO，如java.nio；
- Netty：NIO的客户段/服务器框架，异步的事件驱动的网络应用框架；
- Akka：Scala编写的用于简化编写容错的、高可伸缩性的 Java 和 Scala 的 Actor 模型应用；Akka针对IO操作有一个抽象，和netty一样。使用Akka可以用来创建计算集群,Actor在不同的机器之间传递消息。从这个角度来看,Akka相对于Netty来说，是一个更高层次的抽象。

 

## 代理

客户端 -> 代理 -> 服务器

反向代理 ：代理部署在客户端，属于客户端；

正向代理 ：代理部署在服务端，属于服务端；



## Restful API 及开源框架

### REST

- REpresentational State Transfer：表现层状态转移。URL定位资源，用HTTP动词（GET,POST,DELETE,PUT）描述操作；
- 操作无状态，无需上下文信息；
  - 客户的认证如何实现？让所有的服务都能识别（Token）；
- 不同于传统的PHP、JSP架构（Servlet），**前后端进行分离，统一的接口**为Web、IOS、Android服务；
- JAX-RS -- Java API for RESTful Web Services，于Java EE 6引入（包javax.ws.rs），提供固定的接口开发REST应用，避免第三方框架依赖；
- JAXB（Java Architecture for XML Binding) 是一个业界的标准，是一项可以将XML Schema和Java类互转的技术，成为JDK一部分；

### Restlet 和 Jersey

- JAX-RS规范的实现；
- 通过SPI机制，接口到实现（ServiceLoader.load），可以[手动配置](https://jersey.github.io/documentation/latest/deployment.html#deployment.classpath-scanning)代替自动扫描；
- org.glassfish.jersey.media 的 jersey-media-json-binding 实现json到Java类的转换（writer类的load）；
- 通过自带的Servlet配置Tomcat运行；(**Spring是非标准的JAX-RS的Servlet实现**)

 

### JAX-RS接口注解

- @GET, @POST, @PUT, @DELETE

- @Path("")

- - 引号中若是{year}则表明该值是个变量，在函数的形参中出现，变量名为year并指定类型；

- @PathParam()，@QueryParam(), @DefaultValue(String), @FormParam()

- - @DefaultValue中String会自动类型转换成int等基本类型，应该是调用valueOf方法；
  - @FormParam是将HTML中的表单绑定到方法上，进行取值；



## Java微服务体系

### SpringCloud



### Vert.x（ServiceComb）

在JVM上构建响应式应用程序的工具包。

Apache ServiceComb就是以Vert.x为底层框架实现

- Vert.x单个服务打包完成后大约7M左右的JAR，不依赖Tomcat、Jetty之类的容器，直接在JVM上跑；
- Vert.x实现的开源网关- VX-API-Gateway帮助文档（https://github.com/EliMirren/VX-API-Gateway）；



## Servlet 

> javax.servlet 规范，用于处理 HTTP 请求。

- 实现 Servlet 接口，处理请求；
- 定义`web.xml`，配置 URL 对应的处理的类全名；

一般直接继承 **HttpServlet**。

- **非线程安全**；



## Tomcat

> Tomcat 9

### 基础

- **CATALINA_HOME**指定的是tomcat主目录，**CATALINA_BASE**指定的是实例的目录，添加脚本配置上面环境变量，通过CATALINA_HOME/bin/catalina.sh脚本执行；

- 一个Tomcat是一个JVM进程，会加载webapps下面的所有项目，可以**单机部署多个Tomcat实例**；

- Tomcat类加载器说明：catalina.sh启动
  - Bootstrap、Extension加载器：`$JAVA_HOME/jre/lib/rt.jar`, ​`$JAVA_HOME/jre/lib/ext`

  - System加载器：$CATALINA_HOME/bin下面的jar包

  - Common加载器：$CATALINA_BASE/lib，然后是$CATALINA_HOME/lib；上述webapps都可见

  - WebappX加载器：webapp/classes, webapp/lib；仅当前webapp可见

  **默认的加载顺序是Bootstrap，WebappX， System， Common**；若设置加载器**delegate='true'，则按照从上到下顺序加载**；
  
  **不同的webapp不同的WebappX加载器**；

- Tomcat的默认访问路径以webapps的文件名为base url，里面的web.xml配置的为相对路径；

- **热部署**： 整个项目重新部署，包括你从新打上**.war** **文件。 会清空session** **，释放内存。项目打包的时候用的多。**

  - 在server.xml -> context 属性中 设置 autoDeploy="true"

- **热加载**：服务器会监听 class 文件改变，包括web-inf/class,web-inf/lib,web-inf/web.xml等文件，若发生更改，则局部进行加载，不清空session ，不释放内存。开发中用的多，但是要考虑内存溢出的情况。

  - 在server.xml -> context 属性中 设置 reloadable="true"




### 日志

- 如果log4j依赖jar在Tomcat目录，则Tomcat该进程只会有一个该类的加载，**不同的webapps的log（无论是否声明为static）即使配置不同的输出文件，也会输出到同一个文件中**；

- 使用log4j日志输出，http://tomcat.apache.org/tomcat-8.0-doc/logging.html#Using_Log4j （Tomcat     8.5之后log4j2和logback可以直接插入到java.util.logging中）

- - 将 log4j.properties 和 log4j-1.2.17.jar放到tomcat/lib目录下；
  - 将**tomcat-juli-adapters.jar**包放到tomcat/lib目录下；
  - 将**tomcat-juli.jar**包放到tomcat/bin目录；因为它是用作引导进程的，而引导类都是从 bin 加载的；
  - 删除或重命名tomcat/conf目录下的logging.properties文件；

- Tomcat 的内部日志使用 JULI 组件，这是一个 Apache Commons 日志的重命名的打包分支，默认被硬编码，使用 java.util.logging 架构。这能保证Tomcat 内部日志与 Web 应用的日志保持独立，即使 Web 应用使用的是 Apache Commons Logging。



### NIO

- tomcat 的 **org.apache.coyote.http11.Http11NioProtocol **的Connector是一个使用 Java NIO 实现的异步 accept 请求的 connector；
  - 将 IO 线程和 业务线程分开；
  - 使用固定的 acceptThread (网络 IO 线程, 负责 accept, read, write) 来异步处理(nio accept, read, write) n 个请求, 然后将请求入队, 最后使用固定的requestProcessingThread (业务线程) 来处理业务逻辑, 业务逻辑的处理实际上也是同步的；



## UnderTow

采用 Java 开发的灵活的高性能 Web 服务器，提供包括阻塞和基于 NIO 的非堵塞机制。