[toc]

# Volatile

volatile：加内存同步障，对该变量的读取会从内存读取，而不是用寄存器中的值（防止优化）。

**一写多读场景，可以不用锁，用volatile修饰即可。**

- **引用变量的复制是原子性操作**



指令重排与双重校验单例，通过volatile屏蔽指令重排

```java
public class Singleton {
    private static volatile Singleton instance;

    public static Singleton getSingleton() {
        if (instance == null) { // Single Checked
            synchronized (Singleton.class) {
                if (instance == null) { // Double Checked
                    instance = new Singleton();
                }
            }
        }
        return instance;
    }
}
```

new操作应该是：

1）分配一块内存；

2）在内存上完成Singleton对象的初始化；

3）把内存地址给instance；

但实际上优化后可能是这样的：

1）分配一块内存；

2）将内存地址给instance；

3）在内存上完成Singleton对象的初始化；

我们假设有两个线程A、B，A执行getIntance到指令2时，线程切换到线程B，线程B看到instance已经不为空，认为可用了。但这时还没有进行对象的初始化，这时使用instance，就可能出现问题。

 