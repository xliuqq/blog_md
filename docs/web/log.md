# Logging & Tracing 

Logging：

- 一条一条的记录，而记录本身是离散事件，没有任何直接关系。Logging 可以在 Console、ElasticSearch、Kafka、File 等各种媒介中显示。而 Logging 的格式又可以通过各种 Logging 的实现去定义 Logging 的格式。

Tracing：

- 整个处理链条的间接关系，把各种离散的 Logging 产生联系，让各种处理事件产生一定范围。



## Tracing

