# Tomcat

Tomcat 9

## 基础

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

- - 在server.xml -> context 属性中 设置 autoDeploy="true"

- **热加载**：服务器会监听 class 文件改变，包括web-inf/class,web-inf/lib,web-inf/web.xml等文件，若发生更改，则局部进行加载，不清空session ，不释放内存。开发中用的多，但是要考虑内存溢出的情况。

- - 在server.xml -> context 属性中 设置 reloadable="true"



## 日志

- 如果log4j依赖jar在Tomcat目录，则Tomcat该进程只会有一个该类的加载，**不同的webapps的log（无论是否声明为static）即使配置不同的输出文件，也会输出到同一个文件中**；

- 使用log4j日志输出，http://tomcat.apache.org/tomcat-8.0-doc/logging.html#Using_Log4j （Tomcat     8.5之后log4j2和logback可以直接插入到java.util.logging中）

- - 将 log4j.properties 和 log4j-1.2.17.jar放到tomcat/lib目录下；
  - 将**tomcat-juli-adapters.jar**包放到tomcat/lib目录下；
  - 将**tomcat-juli.jar**包放到tomcat/bin目录；因为它是用作引导进程的，而引导类都是从 bin 加载的；
  - 删除或重命名tomcat/conf目录下的logging.properties文件；

- Tomcat 的内部日志使用 JULI 组件，这是一个 Apache Commons 日志的重命名的打包分支，默认被硬编码，使用 java.util.logging 架构。这能保证Tomcat 内部日志与 Web 应用的日志保持独立，即使 Web 应用使用的是 Apache Commons Logging。



## NIO

- tomcat 的 **org.apache.coyote.http11.Http11NioProtocol **的Connector是一个使用 Java NIO 实现的异步 accept 请求的 connector；
  - 将 IO 线程和 业务线程分开；
  - 使用固定的 acceptThread (网络 IO 线程, 负责 accept, read, write) 来异步处理(nio accept, read, write) n 个请求, 然后将请求入队, 最后使用固定的 requestProcessingThread (业务线程) 来处理业务逻辑, 业务逻辑的处理实际上也是同步的；