# Apache Logging

## Log4j2

### 特性

- log4j2 重写 1.x 版本，包含log4j-core和log4j-api；
  - 采用 LMAX 的无锁队列 **Disruptor**（并发线程中交换数据，有界队列）；
  - 默认在**classpath**下加载log4j2.properties, log4j2.xml, log4j2.json等文件；
- Garbage-free
  - garbage-free in stand-alone applications, and low garbage in web applications；

### 使用

```xml
<dependencies>
  <dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-api</artifactId>
    <version>2.13.0</version>
  </dependency>
  <dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-core</artifactId>
    <version>2.13.0</version>
  </dependency>
</dependencies>
```

Log4j2 API 支持 **Log4j 1.2**, **SLF4J**, **Commons Logging** 和 **java.util.logging (JUL)** APIs。

**Spring**

- 和Spring进行单元测试的时候，Junit加载spring的runner（SpringJUnit4ClassRunner）要优先于spring加载log4j，需要把log4j.properties放在resource目录下，才会默认加载；
- 运行时，web.xml配置log4j配置文件目录log4jConfigLocation和log4j监听器

```xml
<listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>
```

### 配置

`log4j.properties`

#### RootLogger

```properties
# 根Logger，负责处理日志记录的大部分日志
# level是日志级别，appenderName是日志输出目的地
log4j.rootLogger = [level], [appendName1], [appendName2]
# 子包org.exam的输出信息，和rootLogger没有关系
log4j.logger.org.exam=DEBUG
# 设置子logger只在自己的appender里输出，默认为false
log4j.additivity.org.exam=false
```

#### Appender

```properties
# 日志信息输出目的地Appender
log4j.appender.[appenderName] = [fully.qualified.name.of.appender.class]
```

##### org.apache.log4j.ConsoleAppender

```properties
# 可以修改输出日志的级别
log4j.appender.Console.Threshold = WARN   
# 默认值是true,意谓着所有的消息都会被立即输出。 
log4j.appender.Console.ImmediateFlush=true 
# 默认情况下是：System.out,指定输出控制台 
log4j.appender.Console.Target=System.err
```

##### org.apache.log4j.RollingFileAppender 

```properties
# MaxFileSize参数指定文件阈值，MaxBackupIndex表示最多存储的日志文件数
log4j.appender.RollingFile.File = /tmp/log
log4j.appender.RollingFile.MaxFileSize=100MB
log4j.appender.RollingFile.MaxBackupIndex=10
```



#### Layout

##### org.apache.log4j. PatternLayout 

##### 灵活指定布局

```properties
log4j.appender.[appenderName].layout = [fully.qualified.name.of.layout.class ]
log4j.appender.[appenderName].layout.ConversionPattern = %d{yyyy-MM-dd HH:mm:ss}  [ %C.%M:%l ] - [ %p ]  %m%n
# 参数详细解释如下
%p: 输出日志信息优先级，即DEBUG，INFO，WARN，ERROR，FATAL, 

%d: 输出日志时间点的日期或时间，默认格式为ISO8601，或指定格式如 %d{yyy MMM dd HH:mm:ss,SSS}，输出类似：2002年10月18日 22:10:28,921 

%r: 输出自应用启动到输出该log信息耗费的毫秒数 

%c: 输出日志信息所属的类目，通常就是所在类的全名 （通过ClassName.class传给Logger识别）

%C: 列出调用logger的类的全名（包含包路径）

%x: 按NDC（Nested Diagnostic Context，线程堆栈）顺序输出日志

%X: 按MDC（Mapped Diagnostic Context，线程映射表）输出日志。通常用于多个客户端连接同一台服务器，方便服务器区分是那个客户端访问留下来的日志。

%t: 输出产生该日志事件的线程名 

%l: 输出日志事件的发生位置，相当于%C.%M(%F:%L)的组合,包括类名、方法名以及在代码中的行数。举例：Testlog4.main(TestLog4.java:10) 

%%: 输出一个"%"字符 

%F: 输出日志消息产生时所在的文件名称 

%L: 输出代码中的行号 

%M: 输出日志所在的函数名

%m: 输出代码中指定的消息,产生的日志具体信息 

%n: 输出一个回车换行符，Windows平台为"\r\n"，Unix平台为"\n"输出日志信息换行 
```



## log4cxx



## Log4j Audit



## Log4j Scala