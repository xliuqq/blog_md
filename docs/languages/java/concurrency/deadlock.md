[toc]

# 死锁

## 静态初始化死锁

### 示例1

**类A的静态初始化涉及到多个线程，且多个线程中又会调用类A的静态初始化或静态方法；**

(示例1：lambda方法被编译成类的静态方法，多线程执行静态方法会等待类初始化完成，导致死锁）

```java
import java.util.stream.IntStream;

public class StaticInitialDeadLockDemo {
    static {
        long count = IntStream.range(0, 10000).parallel().map(i -> i).count();
        System.out.println("done:" + count);
    }

    public static void main(final String[] args) {}
}

```

### 示例2

类A的静态初始化和类B的静态初始化互相引用，不同的线程中同时调用类A和类B的初始化；（示例2）

```java
public class StaticInitialDeadLockDemo2 {
    public static void main(String[] args) throws Exception {
        System.out.println("Main begin...");
        new Thread(() -> System.out.println(Env.EVN_STR)).start();
        System.out.println(Constants.CONSTANTS_STR);
        System.out.println("Main end");
    }

    public static final class Env {
        static final StringBuilder EVN_STR = new StringBuilder("env str");

        static {
            System.out.println("Env static block begin, " + Thread.currentThread());
            // deadlock by Constants
            System.out.println("Env refer -> " + Constants.CONSTANTS_STR);
            System.out.println("Env static block end");
        }
    }

    public static class Constants {
        static final StringBuilder CONSTANTS_STR = new StringBuilder("constants str");

        static {
            System.out.println("Constants static block begin, " + Thread.currentThread());
            // deadlock by Env
            System.out.println("Constants refer -> " + Env.EVN_STR);
            System.out.println("Constants static block end");
        }
    }
}
```

### 原理

 见 [https://docs.oracle.com/javase/specs/jls/se8/html/jls-12.html#jls-12.4.2      ](https://docs.oracle.com/javase/specs/jls/se8/html/jls-12.html#jls-12.4.2)

**For each class or interface C, there is a unique initialization lock LC** : 

1. Synchronize on the initialization lock, LC, for C. This involves waiting until the current thread can acquire LC.
2. If the Class object for C indicates that initialization is in progress for C by some other thread, then release LC and block the current thread until informed that the in-progress initialization has completed, at which time repeat this step.
3. If the Class object for C indicates that initialization is in progress for C by the current thread, then this must be a recursive request for initialization. Release LC and complete normally.
4. If the Class object for C indicates that C has already been initialized, then no further action is required. Release LC and complete normally.