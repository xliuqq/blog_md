# GC与内存



## 引用类型

### 强引用

普通对象引用，有强引用指向一个对象，就表明此对象还“活着”，无法被 GC。

### 软引用

> 描述一些还有用但并非必需的对象

当JVM认定**内存空间不足时才会去回收**软引用指向的对象

- 使用软引用的时候**必须检查引用是否为null**；

```java
Object obj = new Object();
SoftReference<Object> sf = new SoftReference<Object>(obj);
obj = null;
// 有时候会返回null
sf.get();　
```

### 弱引用

> 弱引用与软引用最大的区别就是弱引用比软引用的生命周期更短暂。

垃圾回收器会扫描它所管辖的内存区域的过程中，只要发现弱引用的对象，**不管内存空间是否有空闲，都会立刻回收**它。

```java
Object obj = new Object();
WeakReference<Object> wf = new WeakReference<Object>(obj);
obj = null;
//有时候会返回null
wf.get();
//返回是否被垃圾回收器标记为即将回收的垃圾
wf.isEnQueued();
```

### 幻象引用

幻象引用并不会决定对象的生命周期。即如果一个对象仅持有虚引用，就相当于没有任何引用一样，在任何时候都可能被垃圾回收器回收。

- 不能通过它访问对象

```java
Object obj = new Object();
PhantomReference<Object> pf = new PhantomReference<Object>(obj);
obj=null;
//永远返回null
pf.get();
//返回是否从内存中已经删除
pf.isEnQueued();　
```





## 内存布局

-Xmx -Xms

-XX:InitialRAMPercentage、-XX:MaxRAMPercentage 和 -XX:MinRAMPercentage 





## 查看JDK默认的GC策略

```bash
$ java -XX:+PrintCommandLineFlags -version
```



## GC策略

### ParNew(并行回收)

**新生代的默认垃圾收集器**

ParNew收集器采用**并行回收**的方式执行内存回收，复制算法`、"`Stop-the-World"机制。



### Parallel Scavenge(吞吐量优先，JDK 8默认)

 自适应调节策略：-XX:+UseAdaptiveSizePolicy



### Concurrent-Mark-Sweep(低延迟，JDK9废弃)

 CMS 是一种高度可配置的复杂算法，因此给 JDK 中的 GC代码库带来了很多复杂性。



### Garbage First（G1，JDK 9默认）

`G1依然属于分代型垃圾回收器`，它会区分年轻代和老年代，年轻代依然有Eden区和Survivor区。但从堆的结构上看，它不要求整个Eden区、年轻代或者老年代都是连续的，也不再坚持固定大小和固定数量。

和之前的各类回收器不同，它同时`兼顾年轻代和老年代`。对比其他回收器，或者工作在年轻代，或者工作在老年代。



### ZGC(低延迟GC)



#### JVM的GC线程数的计算



