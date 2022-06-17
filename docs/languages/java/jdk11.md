[toc]

# Java 11

> JDK 11 作为 JDK 8之后的长期维护版本。



## Stack-Walking API

一个标准API用于访问当前线程栈。



## Process API

ProcessHandle提供了对本地进程的控制，可以监控其存活，查找其子进程，查看其信息，甚至销毁它。非常适合耗时较长的进程调用；



## jshell



## 反应式流 （ Reactive Streams ）

java.util.concurrent.Flow 类，Flow.Publisher、Flow.Subscriber、Flow.Subscription 和 Flow.Processor 等 4 个核心接



## MethodHandles 

改进方法句柄，添加更多静态方法创建不同类型方法句柄

- arrayConstructor：创建指定类型的数组。

- arrayLength：获取指定类型的数组的大小。
- varHandleInvoker 和 varHandleExactInvoker：调用 VarHandle 中的访问模式方法。
- zero：返回一个类型的默认值。
- empty：返 回 MethodType 的返回值类型的默认值。
- loop、countedLoop、iteratedLoop、whileLoop 和 doWhileLoop：创建不同类型的循环，包括 for 循环、while 循环 和 do-while 循环。
- tryFinally：把对方法句柄的调用封装在 try-finally 语句中。



## 局部变量类型推断

```java
var list = new ArrayList<String>(); 
```



## 应用程序类数据共享

Class Data Sharing特性在原来的 bootstrap 类基础之上，扩展加入了应用类的 CDS (Application Class-Data Sharing) 支持；当多个 Java 虚拟机（JVM）共享相同的归档文件时，还可以减少动态内存的占用量，同时减少多个虚拟机在同一个物理或虚拟的机器上运行时的资源占用。



## 线程-局部管控

将允许在不运行全局 JVM 安全点的情况下实现线程回调，由线程本身或者 JVM 线程来执行，同时保持线程处于阻塞状态，这种方式使得停止单个线程变成可能，而不是只能启用或停止所有线程。



## JNI Native Header

当编译 JNI 代码时，已不再需要单独的工具来生成头文件，因为这可以通过 javac 完成。



## 异步非阻塞的Http Client

- 支持异步非阻塞的Http Client，支持HTTP/1.1和HTTP/2；



## GC

并行全垃圾回收器 G1：之前 Java 版本中的 G1 垃圾回收器执行 GC 时采用的是基于单线程标记扫描压缩算法（mark-sweep-compact），采用并行化 mark-sweep-compact 算法，并使用与年轻代回收的相同数量的线程（-XX：ParallelGCThread）；

Epsilon：低开销垃圾回收器，-XX:+UseEpsilonGC；

ZGC：可伸缩低延迟垃圾收集器，--with-jvm-features=zgc；

