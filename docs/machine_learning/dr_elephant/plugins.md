## 指标和启发式规则



资源定义： GB Hours



**使用的资源**

MR：

运行时任务的Yarn容器的大小，mapper和reducer任务的总和



Spark？



**浪费的资源**

一个Task的浪费资源计算公式如下：

```
peak_memory_used = Max(max_physical_memory, virtual_memory/2.1)  ??
wasted_memory = Container_size - peak_memory_used 
wasted_resource = wasted_memory * runtime
```



**任务的时间**

Runtime：运行时间，结束-开始

等待时间的计算公式

```
mapper_wait_time = finish_time_last - ( ideal_start_time + task_runtime_max)
reducer_wait_time = finish_time_last - ( ideal_start_time + task_runtime_max) 
```



#### MR

**Mapper数据倾斜**

将mapper任务分成两组：Statistics.findTwoGroups

计算：第一组数均值，第二组数据均值，总的均值

```
 deviation = abs(avg(group_1) - avg(group_2)) / min(avg(group_1), avg(group_2))
```

计算 deviation 的值，确定级别

max ( min ( 时间devitaion, 任务数 ), min( 任务数，数据量devitation ) )

- 解决任务数少的时候，统计不稳定
- 时间相差大 或 数据量差别大

配置：

- deviation_severity，num_tasks_severity，files_severity

##### Mapper GC

计算：GC的时间 / CPU时间

定义变量：

- avg_gc_time, avg_cpu_time, avg_runtime, avg_cpu_ration
- gc_ratio_severity, runtime_severity

##### Mapper 内存



##### Mapper 性能



##### Mapper Spill



##### Mapper 时间



##### Reducer数据倾斜



##### Reducer GC



##### Reducer内存



##### Reducer时间



##### Shuffle和Sort





#### Spark

##### Spark Event Log Limit

Spark的事件日志传递程序目前无法处理非常大的事件日志文件。需要很长时间来分析它，这可能会危及整个服务器。因此，目前我们为事件日志文件设置了一个限制(**100MB**)，如果日志大小超过这个限制，就会绕过日志获取过程。



##### Spark Executor Load Balance

Spark一次性分配所有资源并在运行过程中不释放资源。为了避免过度使用集群，优化executors的负载平衡情况至关重要。

**计算**：峰值内存、最低内存和平均内存之间的偏差系数。

峰值内存、运行时间、输入数据量、输出数据量

**配置参数**：looser_metric_deviation_severity 和 metric_deviation_severity 



##### Spark Job Runtime

一个应用拆分成多个job，一个job拆分成多个Stage



配置：**single_job_failure_rate_severity** ，**avg_job_failure_rate_severity**



##### Spark Memory Limit



**Spark Stage Limit**