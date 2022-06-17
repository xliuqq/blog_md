[toc]

# Metrics

## Metircs4J

kairosdb项目中抽象出来的metrics库。

TODO：https://github.com/kairosdb/metrics4j



## Dropwizard Metrics

[Dropwizard Metrics](https://link.jianshu.com?t=http://metrics.dropwizard.io) 能够从**各个角度度量已存在的java应用**的成熟框架，简便地以**jar包的方式集成**进系统，可以以http、ganglia、graphite、log4j等方式提供全栈式的监控视野。

2020/6/15版本：4.1.9

 TODO：使用时再进一步分析 https://metrics.dropwizard.io/4.1.2/manual/core.html

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
// 静态方法，生成唯一的名，"java.lang.String.jobs.size"
MetricRegistry.name(String.class, "jobs", "size")
```

### Console Reporter

**Reporter会定时去MetricRegistry中获取metric结果**。

```java
ConsoleReporter reporter = ConsoleReporter.forRegistry(metrics)
       .convertRatesTo(TimeUnit.SECONDS)
       .convertDurationsTo(TimeUnit.MILLISECONDS)
       .build();
reporter.start(1, TimeUnit.SECONDS); // 每秒将结果输出到控制台
```

### Meters

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

### Gauges

guage: 量表是对**一个值的瞬时测量**。

```java
// 只是注册Guage这个metric，Reporter获取的时候才会触发计算
metrics.register(MetricRegistry.name(String.class, "test", "size"),
        (Gauge<Integer>) () -> 5);
```

### Counters

是一个`AtomicLong`实例的gauge，可以执行increment 和 decrement函数。

```java
// 使用 #counter(String) 而不是 #register(String, Metric)
Counter pendingJobs = metrics.counter(name(String.class, "pending-jobs"));
pendingJobs.inc(); pendingJobs.dec();
```

### Histograms

直方图**测量数据流中值的统计分布**。除了最小值、最大值、平均值等，它还测量中位数、第75、90、95、98、99和99.9个百分点。

```java
private final Histogram responseSizes = metrics.histogram(name(String.class, "rsizes"));

public void handleRequest(Request request, Response response) {
    // etc
    responseSizes.update(response.getContent().length);
}
```

### Timers

计时器**测量调用特定代码段的速率及其持续时间的分布**，包含Meter和Historm。

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
public class DatabaseHealthCheck extends HealthCheck {
    private final Database database; // Database是用户服务类

    public DatabaseHealthCheck(Database database) {
        this.database = database;
    }

    @Override
    public HealthCheck.Result check() throws Exception {
        if (database.isConnected()) {
            return HealthCheck.Result.healthy();
        } else {
            return HealthCheck.Result.unhealthy("Cannot connect to " + database.getUrl());
        }
    }
}
// 定义HealthCheckRegistry
final HealthCheckRegistry healthChecks = new HealthCheckRegistry();
healthChecks.register("postgres", new DatabaseHealthCheck(database));
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

### Reporting Via JMX

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

### Reporting Via HTTP

```xml
<dependency>
    <groupId>io.dropwizard.metrics</groupId>
    <artifactId>metrics-servlets</artifactId>
    <version>${metrics.version}</version>
</dependency>
```

AdminServlet 提供 所有注册metrics的Json表示；也会运行健康检查；打印thread dump；对load-balancers提供简单的'ping'返回。

### Other Reporting

- `STDOUT`, using [ConsoleReporter](https://metrics.dropwizard.io/4.1.2/manual/core.html#man-core-reporters-console) from `metrics-core`
- `CSV` files, using [CsvReporter](https://metrics.dropwizard.io/4.1.2/manual/core.html#man-core-reporters-csv) from `metrics-core`
- SLF4J loggers, using [Slf4jReporter](https://metrics.dropwizard.io/4.1.2/manual/core.html#man-core-reporters-slf4j) from `metrics-core`
- Graphite, using [GraphiteReporter](https://metrics.dropwizard.io/4.1.2/manual/graphite.html#manual-graphite) from `metrics-graphite`



