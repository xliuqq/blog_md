# Future

异步执行的结果，可与ExecutorService线程池使用；**不提供回调函数**



# CompletableFuture 

CompletableFuture 用then，when等操作来防止Future的阻塞和轮询isDone的现象出现。

- 异步方法都可以指定一个线程池作为任务的运行环境。如果没有指定就会使用ForkJoinPool线程池来执行。

```java
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CountDownLatch;

public class CompletableFutureDemo {

    public static void main(String[] args) throws InterruptedException {
        long l = System.currentTimeMillis();
        CountDownLatch countDownLatch = new CountDownLatch(1);
        CompletableFuture<Integer> completableFuture = CompletableFuture.supplyAsync(() -> {
            System.out.println("执行耗时操作...");
            timeConsumingOperation();
            return 100;
        });
        completableFuture.whenComplete((result, e) -> {
            System.out.println("结果：" + result);
            countDownLatch.countDown();
        });
        System.out.println("主线程运算耗时:" + (System.currentTimeMillis() - l) + " ms");
        countDownLatch.await();
    }

    static void timeConsumingOperation() {
        try {
            Thread.sleep(3000);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```

除了串行执行外，多个`CompletableFuture`还可以并行执行。

`anyOf()`可以实现“任意个`CompletableFuture`只要一个成功”，`allOf()`可以实现“所有`CompletableFuture`都必须成功”，这些组合操作可以实现非常复杂的异步流程控制。

最后我们注意`CompletableFuture`的命名规则：

- `xxx()`：表示该方法将继续在已有的线程中执行；
- `xxxAsync()`：表示将异步在线程池中执行。