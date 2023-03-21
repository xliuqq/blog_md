# Metrics

> [示例代码](https://gitee.com/oscsc/web-tech/tree/master/metrics)

## Dropwizard Metrics

> Spark 使用其作为指标框架。

[Dropwizard Metrics](http://metrics.dropwizard.io) 能够从**各个角度度量已存在的java应用**的成熟框架，简便地以**jar包的方式集成**进系统，可以以http、ganglia、graphite、log4j等方式提供全栈式的监控视野。

2023/3/15版本：4.2.17

### Maven

```xml
<dependencies>
    <dependency>
        <groupId>io.dropwizard.metrics</groupId>
        <artifactId>metrics-core</artifactId>
        <version>${metrics.version}</version>
    </dependency>
    
    <dependency>
        <groupId>io.dropwizard.metrics</groupId>
        <artifactId>metrics-healthchecks</artifactId>
        <version>${metrics.version}</version>
    </dependency>
</dependencies>
```

### Registry

度量的核心是**MetricRegistry**类，它是所有应用程序度量的容器，**MetricRegistry是线程安全类**。

**每个registry里的metric都有唯一的名**，以 '.' 分隔，如"thing.count"，**同一个名字对应同一个metric**

```java
// 静态方法，生成唯一的名，"key.jobs.size"
String count = MetricRegistry.name("key", "jobs", "size")
// 注册指标
MetricRegistry metrics = new MetricRegistry();
// 1. register 注册
metrics.register(count, (Gauge<Integer>) () -> 5)
// 2. 通过 gauge, meter, counter 等注册
metrics.gauge("key.jobs.size")
```

### 指标类型

#### Meters

**meter**：**测量事件随时间变化的速率**，以及1分钟、5分钟、15分钟内的**移动平均值**；

```java
private final MetricRegistry metrics = new MetricRegistry();
private final Meter requests = metrics.meter("requests");

// 测试每分钟请求的频率
public void handleRequest(Request request, Response response) {
    requests.mark();
    // etc
}
```

#### Gauges

guage: 量表是对**一个值的瞬时测量**。

```java
// 只是注册Guage这个metric，Reporter获取的时候才会触发计算（即每次都会进行调用，获取最新的值）
metrics.register(MetricRegistry.name(String.class, "test", "size"),
        (Gauge<Integer>) () -> 5);
```

默认提供`JmxAttributeGauge`，`RatioGauge `，`CachedGauge`，`DerivativeGauge`

#### Counters

> 可用于统计单次时间

是一个`AtomicLong`实例的gauge，可以执行increment 和 decrement函数。

```java
// 使用 #counter(String) 而不是 #register(String, Metric)
Counter pendingJobs = metrics.counter(name(String.class, "pending-jobs"));
pendingJobs.inc(); pendingJobs.dec();
```

#### Histograms

> **reservoir sampling**：蓄水池采样算法（避免统计所有的数据）
>
> - UniformReservoir：随机采样
> - SlidingWindowReservoir：只保留最后的 N 个值；
>
> - SlidingTimeWindowArrayReservoir ：滑动窗口采样
> - ExponentiallyDecayingReservoir：默认，指数采样

直方图**测量数据流中值的统计分布**。除了最小值、最大值、平均值等，它还测量中位数、第75、90、95、98、99和99.9个百分点。

```java
private final Histogram responseSizes = metrics.histogram(name(String.class, "rsizes"));

public void handleRequest(Request request, Response response) {
    // etc
    responseSizes.update(response.getContent().length);
}
```

#### Timers

> 用于统计函数的调用次数和平均执行时间

计时器**测量调用特定代码段的速率及其持续时间的分布**，包含 Meter 和 Historm。

```java
private final Timer responses = metrics.timer(name(RequestHandler.class, "responses"));
// 计时器将以纳秒为单位测量处理每个请求所需的时间，并提供每秒请求的速率
public String handleRequest(Request request, Response response) {
    try(final Timer.Context context = responses.time()) {
        // etc;
        return "OK";
    } // catch and final logic goes here
}
```

### Health Checks

检查用户服务的健康状态

```java
// 继承HealthCheck类，实现check方法
// 异步执行，每 2s 执行一次
@Async(period = 2000)
public class ServerHealthCheck extends HealthCheck {
    @Override
    public HealthCheck.Result check() throws Exception {
        double value = Math.random();

        if (value > 0.5) {
            return HealthCheck.Result.healthy();
        } else {
            return HealthCheck.Result.unhealthy("error random");
        }
    }
}

// 定义HealthCheckRegistry，默认 2线程的线程池运行异步的check
final HealthCheckRegistry healthChecks = new HealthCheckRegistry();
healthChecks.register("server.healthy", new ServerHealthCheck());
// 运行所有注册的健康检查
final Map<String, HealthCheck.Result> results = healthChecks.runHealthChecks();
for (Entry<String, HealthCheck.Result> entry : results.entrySet()) {
    if (entry.getValue().isHealthy()) {
        System.out.println(entry.getKey() + " is healthy");
    } else {
        System.err.println(entry.getKey() + " is UNHEALTHY: " + entry.getValue().getMessage());
        final Throwable e = entry.getValue().getError();
        if (e != null) {
            e.printStackTrace();
        }
    }
}
```

Metrics内置`ThreadDeadlockHealthCheck`健康检查，使用Java内置的**线程死锁检测**。



### Reporter

> core 内置 `ConsoleReporter`, `CsvReporter` 和 `Slf4jReporter`

#### Console Reporter

**Reporter会定时去MetricRegistry中获取metric结果**。

```java
ConsoleReporter reporter = ConsoleReporter.forRegistry(metrics)
       .convertRatesTo(TimeUnit.SECONDS)
       .convertDurationsTo(TimeUnit.MILLISECONDS)
       .build();
reporter.start(1, TimeUnit.SECONDS); // 每秒将结果输出到控制台
```

#### Reporting Via JMX

```xml
<dependency>
    <groupId>io.dropwizard.metrics</groupId>
    <artifactId>metrics-jmx</artifactId>
    <version>${metrics.version}</version>
</dependency>
```

```java
final JmxReporter reporter = JmxReporter.forRegistry(registry).build();
reporter.start();
```

**一旦reporter开始，所有注册的metrics都可以通过JConsole或者VisualVM查看。**

#### Reporting Via HTTP

AdminServlet 提供 **所有注册metrics的Json表示**；也会运行健康检查；打印thread dump；对load-balancers提供简单的'ping'返回。

```xml
<dependency>
    <groupId>io.dropwizard.metrics</groupId>
    <artifactId>metrics-servlets</artifactId>
    <version>${metrics.version}</version>
</dependency>
```

#### Other Reporting

- Graphite, using [GraphiteReporter](https://metrics.dropwizard.io/4.1.2/manual/graphite.html#manual-graphite) from `metrics-graphite`



### Instrumenting

> https://metrics.dropwizard.io/4.2.0/manual/servlet.html

对常见的框架，提供注入，自动记录Metrics，如`Ehcache`，`Caffine`，`Apache HttpClient`，`JDBI`，`Log4j`，`LogBack`，`Jetty`，`Jersey 2.x`，`JVM`。

Web-Application的 metrics（`metrics-servlet`）： status codes(meters), the number of active requests(counter), request duration(timer)

- 通过Filter(`com.codahale.metrics.servlet.InstrumentedFilter`)



### 三方库

> https://metrics.dropwizard.io/4.2.0/manual/third-party.html



## [Prometheus Client](https://github.com/prometheus/client_java)

> Prometheus instrumentation library for JVM applications
