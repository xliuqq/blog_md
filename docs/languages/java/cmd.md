[toc]

# CMD

## JAVA_OPTS

### -server

服务器模式启动JAVA，默认启动JVM基本都是-server模式

### 远程调试参数

远程调试（-XDebug -Xrunjdwp:transport=dt_scoket, server=y,address=7899,suspend=n）：本地的为debugger，远程的为debugee

-XDebug 启用调试；

- -Xrunjdwp 加载JDWP的JPDA参考执行实例；1.7之后可以将 -Xrunjdwp: 替换为 -agentlib:jdwp=
- transport：debugee与debuger调试时之间的通讯数据传输方式。 
- server：是否监听debuger的调试请求(作为调试服务器执行)。 
- suspend：是否等待启动，也即设置是否在debuger调试链接建立后才启动debugee JVM。 
- address： 调试服务器监听的端口号

### -cp

- 指定**加载某路径下的所有第三方包**，-Djava.ext.dirs 或者 **java -cp .;c:\classes\myClass.jar;d:\classes\*.jar**

  linux下为 **java -cp .:/home/text/***，注意 * 不包括递归的子目录中的内容。

### OOM时堆快照

-XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=${目录}

- 当JVM发生OOM时，自动生成DUMP文件。
- 如果不指定文件名，默认为：java_<pid>_<date>_<time>_heapDump.hprof

### -jar

```java
java -jar myClass.jar
```

执行该命令时，会用到目录META-INF\MANIFEST.MF文件，在该文件中，有一个叫Main-Class的参数，它说明了java -jar命令执行的类。

## 打包

解压

```css
jar -xvf jar包名字.jar
```



打包

```css
jar -cvfM 合并后的jar包名字.jar .
```