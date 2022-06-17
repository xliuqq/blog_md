# 日志收集

用于将服务（进程）的日志，统一收集，并发送到下游。



## logstash

### Input

input数据输入端，可以接收来自任何地方的源数据

1：file：从文件中读取

2：syslog：监听在514端口的系统日志信息，并解析成RFC3164格式

3：redis：从redis-server list 中获取

4：beat：接收来自Filebeat的事件



### Filter

filter数据中转层，主要进行格式处理，数据类型转换、数据过滤、字段添加，修改等，主要用做过滤

1：grok: 通过正则解析和结构化任何文本。Grok 目前是logstash最好的方式对非结构化日志数据解析成结构化和可查询化。logstash内置了120个匹配模式，满足大部分需求。

2：mutate: 在事件字段执行一般的转换。可以重命名、删除、替换和修改事件字段。

3：drop: 完全丢弃事件，如debug事件。

4：clone: 复制事件，可能添加或者删除字段。

5：geoip: 添加有关IP地址地理位置信息。



### Output

output是logstash工作的最后一个阶段，负责将数据输出到指定位置，兼容大多数应用

1：elasticsearch: 发送事件数据到 Elasticsearch，便于查询，分析，绘图。

2：file: 将事件数据写入到磁盘文件上。

3：mongodb:将事件数据发送至高性能NoSQL mongodb，便于永久存储，查询，分析，大数据分片。

4：redis:将数据发送至redis-server，常用于中间层暂时缓存。

5：graphite: 发送事件数据到graphite

6：statsd: 发送事件数据到 statsd。



