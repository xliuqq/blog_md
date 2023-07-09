# 字节码

## 编译过程

```mermaid
graph LR
	A(Java 源文件)  --> B(词法解析) --token流--> C(语法解析) --> D(语义分析) --> E(生成字节码) --> F(字节码)

```

## 执行

> 热机的流量切换时，防止**冷机状态的服务器无法承载流量**而假死。

字节码通过类加载过程加载到 JVM环境后，才可以执行：

- 解释执行
- JIT 编译执行
- JIT 编译与解释混合执行

```mermaid
graph
    A{已编译} --是-->B(执行编译后的机器码)
    B --> C(方法返回)
    A --否--> D(方法调用计数器加1)
    D --> E{计数是否超过阈值}
    E --是--> F(提交编译请求)
    E --否--> G(解释方法执行)
    G --> C
    H(解释器) --> G
    F --> I(编译器)
    F --> G
    I --> J(后台编译执行)
    J --> K(Code Cache)
    
```





## 字节码增强

### ASM

[**asm**](https://asm.ow2.io/) 提供一个简单易用的library来实现对java字节码的操作，业界的btrace 和 [arthas](https://link.zhihu.com/?target=https%3A//alibaba.github.io/arthas/)均是通过它实现了运行时trace的功能。

- 提供了ClassReader，ClassVisitor，ClassWriter分别对java字节码进行读，遍历，写的操作；
- 用户可以通过实现自己的Visitor对应的VisitXXX方法来达到对指定遍历对象的自定义操作。

#### 使用



### Apache Common BCEL

> [Apache Commons Bytecode Engineering Library](https://github.com/apache/commons-bcel)



#### 使用

```xml
<dependency>
  <groupId>org.apache.bcel</groupId>
  <artifactId>bcel</artifactId>
  <version>6.5.0</version>
</dependency>
```



