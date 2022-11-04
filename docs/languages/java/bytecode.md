# 字节码

## 编译过程



## 字节码增强

### ASM

[**asm**](https://asm.ow2.io/) 提供一个简单易用的library来实现对java字节码的操作，业界的btrace 和 [arthas](https://link.zhihu.com/?target=https%3A//alibaba.github.io/arthas/)均是通过它实现了运行时trace的功能。

- 提供了ClassReader，ClassVisitor，ClassWriter分别对java字节码进行读，遍历，写的操作；
- 用户可以通过实现自己的Visitor对应的VisitXXX方法来达到对指定遍历对象的自定义操作。

#### 使用



### Apache Common BCEL

> Apache Commons Bytecode Engineering Library



#### 使用

```xml
<dependency>
  <groupId>org.apache.bcel</groupId>
  <artifactId>bcel</artifactId>
  <version>6.5.0</version>
</dependency>
```



### AspectJ

[AspectJ](http://www.eclipse.org/aspectj)作为 Java 中流行的 **AOP（aspect-oriented programming）** 编程扩展框架，其内部使用的是 **[BCEL框架](https://github.com/apache/commons-bcel)** 来完成其功能。

- **在一些固定的切入点来进行操作**；
- **匹配规则采用了类似正则表达式的规则**；
- 对于运行中的java进行**无法在不重启的条件下执行新增MOCK**；
- MOCK功能代码嵌入到目标函数中，**无法对MOCK功能代码进行卸载**，可能带来稳定性风险。



### Java Agent

#### 配置

```xml
<!-- maven 打包生成-->
<plugin>
    <groupId>org.apache.maven.plugins</groupId>
    <artifactId>maven-jar-plugin</artifactId>
    <version>2.6</version>
    <configuration>
        <archive>
            <manifestEntries>
                <Agent-Class>com.uber.profiling.Agent</Agent-Class>
                <Premain-Class>com.uber.profiling.Agent</Premain-Class>
            </manifestEntries>
        </archive>
    </configuration>
</plugin>
```

- 在一个普通 Java 程序（带有 main 函数的 Java 类）运行时，通过 -javaagent 参数指定一个特定的 jar 文件（包含 Instrumentation 代理）来启动 Instrumentation 的代理程序。**在类的字节码载入jvm前会调用ClassFileTransformer的transform方法**，从而实现修改原类方法的功能，实现aop；
- 不会像动态代理或者cglib技术实现aop那样会产生一个新类，也不需要原类要有接口；
- META-INF/MAINFEST.INF文件
  - Premain-Class：必须包含，JVM启动时指定代理，包含premain方法的类；
  - Agent-Class：支持VM启动之后，在某时刻启动代理的机制，指定代理类（包含agentmain方法）；
  - Boot-Class-Path：引导类加载器搜索的路径列表；
  - Can-Redefine-Classes：布尔值（默认），能够重定义此代理需要的类；
  - Can-Retransform-Classes：true或false（默认），能否重转换此代理需要的类；
  - Can-Set-Native-Method-Prefix：true 或 false（默认），是否能设置此代理所需的本机方法前缀；

#### 启动时加载(Premain)

通过 ***-javaagent*** 参数指定一个特定的 jar 文件（包含Instrumentation 代理）来启动代理程序；

**META-INF/MAINFEST.INF**文件

- **Premain-Class**：必须包含，**JVM启动时指定代理**，包含**premain**方法的类；

```java
// 先找该方法签名
public static void premain(final String args, final Instrumentation instrumentation) {}
// 找不到，再找该方法函数
public static void premain(String agentArgs);
```

#### 运行时加载(Agentmain)

通过***attach***机制，将JVM A连接至JVM B，并发送指令给JVM B执行。

**META-INF/MAINFEST.INF**文件

- **Agentmain-Class**：必须包含，**JVM启动时指定代理**，包含agentmain方法的类；

```java
public static void agentmain(final String args, final Instrumentation instrumentation) {}
public static void agentmain(String agentArgs);
```

在主程序运行前就指定*javaagent*参数，*premain*方法中代码出现异常会导致主程序启动失败等，为了解决这些问题，JDK1.6以后提供了在**程序运行之后改变程序**的能力。

```java
public static void main(String[] args) {
    try {
        String jvmPid = "23281"; // 目标进行的pid;
        VirtualMachine vm = VirtualMachine.attach(jvmPid);  
        vm.loadAgent("agent.jar");
        vm.detach();
    } catch (Exception e) {
        e.printStackTrace();
    }
}
```

#### Instrument

instrument是JVM提供的一个可以修改已加载类的类库，专门为Java语言编写的插桩服务提供支持。它需要依赖JVMTI的Attach API机制实现。

- 实现它提供的ClassFileTransformer接口，定义一个类文件转换器。接口中的transform()方法会在类文件被加载时调用；
- 在**transform**方法里，我们可以利用上文中的ASM或Javassist对传入的字节码进行改写或替换，生成新的字节码数组后返回。

