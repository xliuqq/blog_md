# 同步&互斥

共享资源的互斥访问

共享资源同步（同步是要保证两个线程事件执行的时序关系）

## synchronized

```java
synchronized {
}
```

## wait & notify

```java
synchronized {
    while (condition is not matched) {
        obj.wait();  // (Release lock, and reacquired on wakeup)
    }
}

synchronized {
    obj.notify();
}
```

- **wait 方法会释放锁**，进行对象的等待池，而 **Thread的sleep，yield方法不会释放锁**；
- **notify()会在同步方法块执行完后，再释放锁**；
- 在没有通知的情况下，等待线程也可能（但很少）会苏醒过来，被称为“伪唤醒”（spurious wakeup）。
- 一般情况下，优先使用notifyAll，而不是notify。

## CAS

> CAS : Compare And Swap，乐观锁机制

Java的原子类AtomicXXX 和 XXXAdder 都是使用CAS保证了线程安全。

- **ABA问题**：我们拿到期望值A，再用CAS写新值C，这两个操作并不能保证原子性。因此，就有可能出现实际值已经改为B，又改回A，这种情况。当执行CAS时，不会识别ABA问题，只要看实际值还是A，就执行了。
- 原子化的对象引用：对象引用都是封装类型，支持CAS操作。相关实现包括：AtomicReference、AtomicStampedReference和AtiomicMarkableReference。AtomicStampedReference和AtiomicMarkableReference解决了ABA问题。
- 原子化数组：AtomicIntegerArray、AtomicLongArray和AtomicReferenceArray；
- 原子化对象属性更新器：AtomicIntegerFieldUpdater、AtomicLongFiledUpdater和AtomicReferenceFieldUpdater
- 原子化的累加器：DoubleAccumulator、DoubleAdder、LongAccumulator 和 LongAdder





## CountDownLatch 

**线程间同步，多用于任务划分场景，标记任务是否完成；**

```java
// ExecutorService用于计时结束
CountDownLatch latch = new CountDownLatch(1);
latch.countDown();
latch.wait();

// 线程间同步
class Driver { // ...
    void main() throws InterruptedException {
        CountDownLatch startSignal = new CountDownLatch(1);
        CountDownLatch doneSignal = new CountDownLatch(N);

        for (int i = 0; i < N; ++i) // create and start threads
            new Thread(new Worker(startSignal, doneSignal)).start();

        doSomethingElse();            // don't let run yet
        startSignal.countDown();      // let all threads proceed
        doSomethingElse();
        doneSignal.await();           // wait for all to finish
    }
}

class Worker implements Runnable {
    private final CountDownLatch startSignal;
    private final CountDownLatch doneSignal;
    Worker(CountDownLatch startSignal, CountDownLatch doneSignal) {
        this.startSignal = startSignal;
        this.doneSignal = doneSignal;
    }
    public void run() {
        try {
            
            
            startSignal.await();
            doWork();
            doneSignal.countDown();
        } catch (InterruptedException ex) {} // return;
    }

    void doWork() { ... }
}
```



- 
