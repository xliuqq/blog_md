[toc]

# HTTP 服务

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