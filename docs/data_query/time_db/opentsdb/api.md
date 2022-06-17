**OpenTSDB 2.3.1版本之后支持HBase 2.x** 



## 使用

**写入**

- Telent，终端；

- HTTP API；
- Batch from file；

**[读取](http://opentsdb.net/docs/build/html/user_guide/query/index.html )**

- CLI tools

- HTTP API 

- GnuPlot graph




## 查询的语法



### Filters

支持对tagV值的过滤（当前不支持对metrics和tagk进行过滤）；

### Aggregation

聚合，将多个时间序列变成单个的数据点；

### DownSampling

下采样，以**一定间隔**采用某种**聚合**函数，返回数据；避免返回数据量过大。

### Rate

以斜率的形式返回，例如计数器，返回的不是绝对值，而是增长值；



**查询处理的顺序：**

1. Filtering
2. Grouping
3. Downsampling
4. Interpolation
5. Aggregation
6. Rate Conversion
7. Functions
8. Expressions



## HTTP API

GET, POST, DELETE 方法

- /api/query/exp
- /api/query/gexp
- /api/query/last