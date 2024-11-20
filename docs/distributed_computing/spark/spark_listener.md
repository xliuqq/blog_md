# Spark 监听器

Spark 中提供了事件监听机制，可以在 Application/Job/Stage/Task 等阶段做自定义的处理逻辑。

```scala
// 执行是异步的，即 caller 发送事件时，处理在另一个线程中，不会阻塞 caller
sparkContext.addSparkListener(new SparkListener() extends SparkListenerInterface {
	// job启动的事件
	override def onJobStart(jobStart: SparkListenerJobStart): Unit = { }
    ... ...
})
```

SparkContext 的事件监听器是由 `LiveListenerBus`管理，`LiveListenBus`默认提供四个监听器总线：

- `shared`：所有的不是Spark内部的监听器共享的总线，通过 SparkContext 注册的都有其处理；
- `appStatus`：应用状态总线；
- `executorManagement`：executor管理总线，关心executor上下线等事件（如用于检测 idle executor）；
- `eventLog`：日志事件总线；

每个总线类型是`AsyncEventQueue`，其基类是`SparkListenerBus`

```scala
// 监听器类型是 SparkListenerInterface，处理事件是 SparkListenerEvent
trait SparkListenerBus extends ListenerBus[SparkListenerInterface, SparkListenerEvent] {
  // 继承 ListenerBus 调用 listener 的 onXXX 函数，同步调用
  protected override def doPostEvent( listener: SparkListenerInterface,
      event: SparkListenerEvent): Unit = { ... ... }
}

trait ListenerBus[L <: AnyRef, E] extends Logging {
    // 内部维护了所有的 listener列表（并发安全）
    
    // 给所有的 listener 广播事件，调用 doPostEvent 方法，调用者应该在一个线程中
    def postToAll(event: E): Unit = { ... ... }
    
    // 子类实现
	protected def doPostEvent(listener: L, event: E): Unit
}
```

`AsyncEventQueue`内部维护了`LinkedBlockingQueue`，其容量默认是10000，**异步事件处理**：

- `spark.scheduler.listenerbus.eventqueue.$name.capacity`：该参数配置容量大小，`$name`是上卖弄四个名字之一；
- `post`方法，将事件加入队列，超过容量（无法添加）则丢弃并记录日志；
- 提供单独的线程，执行`dispatch`方法，从队列中阻塞性获取事件（拿不到则等待）；

`AsyncEventQueue`中线程**停止的逻辑**，是通过放入**`POISON_PILL`事件**（一个特殊事件）实现：

- 保证内部所有的事件都被处理完，因为`take`会阻塞等待。

```scala
private def dispatch(): Unit = LiveListenerBus.withinListenerThread.withValue(true) {
  var next: SparkListenerEvent = eventQueue.take()
  while (next != POISON_PILL) {
    val ctx = processingTime.time()
      try {
        super.postToAll(next)
      } finally {
        ctx.stop()
      }
    eventCount.decrementAndGet()
    next = eventQueue.take()
  }
  eventCount.decrementAndGet()
}

private[scheduler] def stop(): Unit = {
  if (!started.get()) {
    throw new IllegalStateException(s"Attempted to stop $name that has not yet started!")
  }
  if (stopped.compareAndSet(false, true)) {
    eventCount.incrementAndGet()
    eventQueue.put(POISON_PILL)
  }
  // This thread might be trying to stop itself as part of error handling -- we can't join
  // in that case.
  if (Thread.currentThread() != dispatchThread) {
    // If users don't want to wait for the dispatch to end until all events are drained,
    // they can control the waiting time by themselves.
    // By default, the `waitingTimeMs` is set to 0,
    // which means it will wait until all events are drained.
    val exitTimeoutMs = conf.get(LISTENER_BUS_EXIT_TIMEOUT)
    dispatchThread.join(exitTimeoutMs)
  }
}
  
val POISON_PILL = new SparkListenerEvent() { }
```

