# Spark Monitor

***RSS/VM 内存***：PID 为 executor进程的**父PID**，搜索其所有的子PID

- java / python / other：根据进程名区分，



## Metrics

> 每个进程都会初始化 MetricsSystem
>
> - SparkEnv 会初始化 MetricsSystem，但并不启动；
>   - Driver：SparkContext 创建Driver env，并启动每个配置的 Sink（将 servlet的 sink 添加到 UI 中）；
>   - Executor：CoarseGrainedExecutorBackend 创建 Executor Env，并启动每个配置的 Sink（但并没有UI，无法添加 servlet）；
> - ExternalShuffleService
> - Standalone Master
> - Standlone Worker

有两种，一种在Driver端（通过心跳获取，包括Driver/Executor的metrics），另一种则是每个进程单独的Metrics。

### Driver UI

- `/metrics/executors/prometheus`或者`/api/v1/applications/[app-id]/executors`：只能提供内存的峰值，通过`ExecutorSummary`获取；

```txt
spark_info{version="3.0.2", revision="648457905c4ea7d00e3d88048c63f360045f0714"} 1.0
... ...
... ...
metrics_executor_ProcessTreeJVMVMemory_bytes{application_id="app-20221118102536-0010", application_name="Spark Pi", executor_id="0"} 7801798656
metrics_executor_ProcessTreeJVMRSSMemory_bytes{application_id="app-20221118102536-0010", application_name="Spark Pi", executor_id="0"} 710107136
metrics_executor_ProcessTreePythonVMemory_bytes{application_id="app-20221118102536-0010", application_name="Spark Pi", executor_id="0"} 0
metrics_executor_ProcessTreePythonRSSMemory_bytes{application_id="app-20221118102536-0010", application_name="Spark Pi", executor_id="0"} 0
```

### Metrics Sink

```
*.sink.prometheusServlet.class=org.apache.spark.metrics.sink.PrometheusServlet
*.sink.prometheusServlet.path=/metrics/prometheus
master.sink.prometheusServlet.path=/metrics/master/prometheus
applications.sink.prometheusServlet.path=/metrics/applications/prometheus
```

- Sink：能够拿到实时内存值，但需要每个executor单独获取；
  - `ConsoleSink`，`CSVSink`：driver/executor 都会执行；
  - `MetricsServelt`，`PrometheusServlet` ：只有Driver有UI，才能通过Rest接口访问，Executor的数据无法访问；

```txt
metrics_app_20221118102536_0010_driver_ExecutorMetrics_ProcessTreeJVMRSSMemory_Number{type="gauges"} 2964197376
metrics_app_20221118102536_0010_driver_ExecutorMetrics_ProcessTreeJVMRSSMemory_Value{type="gauges"} 2964197376
metrics_app_20221118102536_0010_driver_ExecutorMetrics_ProcessTreeJVMVMemory_Number{type="gauges"} 18204434432
metrics_app_20221118102536_0010_driver_ExecutorMetrics_ProcessTreeJVMVMemory_Value{type="gauges"} 18204434432
metrics_app_20221118102536_0010_driver_ExecutorMetrics_ProcessTreeOtherRSSMemory_Number{type="gauges"} 0
```

### 自定义SInk

#### Prometheus 实时值方案

- [Prometheus gateway push sink](https://github.com/banzaicloud/spark-metrics)：通过自定义Sink实现向Prometheus Gateway 推送；

  

### 自定义 Source
> [Demo 示例](https://gitee.com/oscsc/bigdatatech/blob/master/spark/metrics-source/README.md)

```
spark.metrics.conf.[component_name].source.jvm.class=[source_name]
// 例如 spark 自带的
"spark.metrics.conf.*.source.jvm.class" = "org.apache.spark.metrics.source.JvmSource"
```
