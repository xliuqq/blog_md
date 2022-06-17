[toc]

## 同步&互斥

共享资源的互斥访问

共享资源同步（同步是要保证两个线程事件执行的时序关系）

### synchronized

```java
synchronized {
}
```

### wait & notify

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

### CountDownLatch 

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



## Lock & ReentrantLock

Lock 提供**无条件、可轮询、定时、可中断**的锁，解决了内置锁的问题：

- 无法中断一个正在等待获取锁的线程，无法在请求获取一个锁时无限地等待下去；

ReentrantLock：可**重入的加锁**语义；

- 
