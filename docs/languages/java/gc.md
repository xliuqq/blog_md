# GC与内存

## 内存配置

-Xmx -Xms

-XX:InitialRAMPercentage、-XX:MaxRAMPercentage 和 -XX:MinRAMPercentage 



## 内存分区



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



