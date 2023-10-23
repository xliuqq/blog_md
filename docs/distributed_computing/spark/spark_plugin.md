# Spark Plugin

`SparkPlugin`接口定义两个函数：

- 可以用来启动服务；
- 支持自定义 metrics；

```scala
DriverPlugin driverPlugin();

ExecutorPlugin executorPlugin();
```



## Driver Plugin

```scala
public interface DriverPlugin {
  default Map<String, String> init(SparkContext sc, PluginContext pluginContext) {
    return Collections.emptyMap();
  }
  default void registerMetrics(String appId, PluginContext pluginContext) {}
  default Object receive(Object message) throws Exception {
    throw new UnsupportedOperationException();
  }
  default void shutdown() {}
}
```



## Executor Plugin

```scala
public interface ExecutorPlugin {
  /** 
   * block executor initialization until it returns.
   * register all metrics with the context's registry
   */
  default void init(PluginContext ctx, Map<String, String> extraConf) {}
  default void shutdown() {}
  
  // 可以通过 TaskContext.get 获取当前执行的 Task 信息，注意一个Executor可以并行运行多个Task
  default void onTaskStart() {}
  default void onTaskSucceeded() {}
  default void onTaskFailed(TaskFailedReason failureReason) {}
}
```

