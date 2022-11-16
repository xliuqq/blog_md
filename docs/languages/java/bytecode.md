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


