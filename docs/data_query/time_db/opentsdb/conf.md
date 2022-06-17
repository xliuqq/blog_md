# 配置

## 基础配置

- **tsd.mode ** ：`rw`，TSD的工作模式，是否允许数据点的写入（rw和ro），元数据不受影响；
- **tsd.uid.use_mode**：False，确定是否应根据tsd.mode设置填充名称和/或ID缓存；



- **tsd.core.auto_create_metrics** ：False, 新的metrics是否自动分配UID；
- **tsd.core.uid.random_metrics** ：False，随机分配到UIDs到新的metrics；
- **tsd.core.auto_create_tagks** ：True，新的tag name是否自动分配UID；
- **tsd.core.auto_create_tagvs** ：True，新的tag value是否自动分配UID；
- **tsd.core.connections.limit** ：0，一个TSD最多处理的连接数；
- **tsd.core.enable_api ** ：True，新的2.x的HTTP API；
- **tsd.core.enable_ui** ：True，内置的GUI界面和旧HTTP API；
- **tsd.core.histograms.config** ：2.4, TODO

- **tsd.core.plugin_path** : 插件的位置

- **tsd.core.stats_with_port** ：是否暴露stats；

- **tsd.core.tag.allow_specialchars** ：metrics名称和tag的KV中允许的特殊字符；
- **tsd.core.timezone** ：系统默认，时区；
- **tsd.no_diediedie** ：False，是否允许`diediedie`的HTML和ASCII命令关闭TSD；

- **tsd.core.preload_uid_cache** : False, TSD启动时加载uid缓存；
- **tsd.core.preload_uid_cache.max_entries** ：300000，uid预先缓存时扫描的行数；



- **tsd.core.storage_exception_handler.enable** ：False，允许存储的异常处理

- **tsd.core.storage_exception_handler.plugin** ：存储的异常处理的类名

- **tsd.core.authentication.enable** ：False，允许认证
- **tsd.core.authentication.plugin** ：认证的类名

- **tsd.rtpublisher.enable**：False，允许实时的发布插件；
- **tsd.rtpublisher.plugin**：实时发布的插件类名；
- **tsd.search.enable**：False，允许搜索功能；
- **tsd.search.plugin**：搜索功能插件；

- **tsd.uidfilter.enable**：False，启动UID赋值过滤插件；

- **tsd.uidfilter.plugin**：UID过滤插件类名；

- **tsd.startup.enable**：False，启动TSD前是否加载startup插件；
- **tsd.startup.plugin**：startup插件类名；
- **tsd.timeseriesfilter.enable**：False，允许数据点写时过滤插件；
- **tsd.timeseriesfilter.plugin**：写时过滤插件类名；



- **tsd.core.tree.enable_processing**：TODO



- **tsd.stats.canonical**： TODO

- **tsd.uid.lru.enable**：False，将UID的无限制大小的map改为lru 缓存；

- **tsd.uid.lru.id.size**：5000000，UID到String的映射的最大条目数；
- **tsd.uid.lru.name.size**：5000000，String到UID的映射的最大条目数；



## HTTP配置

- **tsd.http.cachedir** ：临时文件写的路径
- **tsd.http.query.allow_delete** ：查询期间是否允许删除

- **tsd.core.bulk.allow_out_of_order_timestamps**

- **tsd.http.header_tag** ：Put API 时从HTTP Header解析并存储到tag的KV中；
- **tsd.http.request.cors_domains** ：CORS；
- **tsd.http.request.cors_headers** ：执行cors请求，返回给客户端的头域；
- **tsd.http.request.enable_chunked** ：HTTP RPC是否支持chunk；
- **tsd.http.request.max_chunk** ：4096，支持chunk时最大的请求体大小；
- **tsd.http.rpc.plugins** ：TSD启动时加载的RPC的插件的类名；
- **tsd.http.show_stack_trace** ：False，请求异常时是否返回异常堆栈；
- **tsd.http.staticroot** ：静态文件如js，css的路径；



## 网络设置

- **tsd.network.async_io** ：True，使用NIO还是BIO；
- **tsd.network.backlog** ：默认由'somaxconn'或Netty设置，完成或未完成的连接请求的连接队列的深度；
- **tsd.network.bind|port** ：0.0.0.0，绑定的IP地址 | 端口号；
- **tsd.network.keep_alive** ：True，允许keepalive连接；
- **tsd.network.reuse_address** ：True，允许在Netty内重用绑定的端口；
- **tsd.network.tcp_no_delay** ：True，是否在发送数据前禁用TCP缓冲；
- **tsd.network.worker_threads** ：#CPU Cores * 2，Netty的异步的IO工作线程数；
- **tsd.rpc.plugins** : RPC插件的类名；
- **tsd.rpc.telnet.return_errors** :



## MetaData

- **tsd.core.meta.cache.enable** ： False，是否允许元数据缓存(meta data cache)；

- **tsd.core.meta.cache.plugin** : 元数据缓存的插件的类名；
- **tsd.core.meta.enable_realtime_ts **： False，允许实时的TSMeta对象的创建；
- **tsd.core.meta.enable_realtime_uid** ：False，允许实时的UIDMeta对象的创建；
- **tsd.core.meta.enable_tsuid_incrementing** ：TODO

- **tsd.core.meta.enable_tsuid_tracking** ： TODO



## 查询相关的配置

- **tsd.query.enable_fuzzy_filter** ：True，使用`explicitTags`时允许HBase的FuzzyRowFilter；
- **tsd.query.allow_simultaneous_duplicates **：False，是否允许来自同一主机的同时重复查询；
- **tsd.query.filter.expansion_limit**：
- **tsd.query.limits.bytes.allow_override **：
- **tsd.query.limits.bytes.default** ：
- **tsd.query.limits.data_points.allow_override** ：False，
- **tsd.query.limits.data_points.default** ：0，从storage获取的数据点的限制，达到限制时抛出异常；
- **tsd.query.limits.overrides.interval**
- **tsd.query.limits.overrides.config**
- **tsd.query.multi_get.enable** ：False，是否和搜索插件一起启用multi_get；
- **tsd.query.multi_get.batch_size** ：1024，单个请求中，发送到storage的`get`请求个数；
- **tsd.query.multi_get.concurrent** ：20，
- **tsd.query.multi_get.get_all_salts** ：TODO
- **tsd.query.skip_unresolved_tagvs** ：False，查询包含tag value没有对应UID时是否继续查询；
- **tsd.query.timeout** ：0，查询的超时（ms）



## RollUps

- **tsd.rollups.config**：rollup和aggregation的配置文件地址；
- **tsd.rollups.enable**：False，启用汇总和预聚合存储和写入；
- **tsd.rollups.tag_raw**：False，是否使用在tsd.rollups.agg_tag_key中配置的标记键和在tsd.rollups.raw_agg_tag_value中配置的值标记未汇总和未预汇总的值；
- **tsd.rollups.agg_tag_key**：_aggregate，在写入存储时用以标记预聚合数据的特殊键；
- **tsd.rollups.raw_agg_tag_value**：RAW，写入存储时对非汇总和非预聚合数据的特殊标记值；
- **tsd.rollups.block_derived**：False，是否阻止存储派生聚合，如AVG和DEV；



## 存储配置

- **tsd.storage.compaction.flush_interval **：10，在压缩队列flush调用期间等待的时间(s)；
- **tsd.storage.compaction.flush_speed** ：2，确定尝试刷新压缩队列的速度(1h/2=0.5h)；
- **tsd.storage.compaction.max_concurrent_flushes**：10000，在任何给定时间内对HBase进行压缩调用的最大数目；
- **tsd.storage.compaction.min_flush_threshold**：100，触发刷新前必须超过的压缩队列大小；
- **tsd.storage.enable_appends**：False，写数据点时采用append到列，而不是创建新列的方式（避免每小时的压缩，但使用更多资源）；
- **tsd.storage.enable_compaction**：True，是否允许压缩；
- **tsd.storage.fix_duplicates**：False，分析具有重复时间戳的数据点时是否接受上次写入的值。与压缩一起启用时，将使用最新的数据点写入压缩列；
- **tsd.storage.flush_interval**：1000，刷新数据点存储写入缓冲区的间隔（ms）；
- **tsd.storage.hbase.data_table** ：tsdb，数据点存储的HBase的表名；
- **tsd.storage.hbase.meta_table** ：tsdb-meta，元数据存储的HBase的表明；
- **tsd.storage.hbase.prefetch_meta** ：False，是否在启动网络接口之前预取TSDB表的region信息；
- **tsd.storage.hbase.scanner.maxNumRows**：128，每次调用scanner的nextRows方法从HBase获取的最大行数；
- **tsd.storage.hbase.tree_table** ：tsdb-tree，tree data存储的表；
- **tsd.storage.hbase.uid_table** ：tsdb-uid，uid信息存储的表；
- **tsd.storage.hbase.zk_basedir** ：/hbase，
- **tsd.storage.hbase.zk_quorum** ：localhost，zookeeper地址；
- **tsd.storage.repair_appends** ：False，当列包含重复或无序数据时，是否在查询时重新写入追加的数据点列；
- **tsd.storage.max_tags** ：8，每个数据点允许的最大标记数。此值可以在创建群集后更改。请注意过度使用标记写入的性能权衡；
- **tsd.storage.salt.buckets** 和 **tsd.storage.salt.width**
  - salt 配置：这两个配置在**OpenTSDB初始化就必须确定**，运行过程中不能随意修改，否则会导致历史数据读取错误。
  - tsd.storage.salt.width ：0，SALT的字节长度。当设置为0时，表示不使用SALT。
  - tsd.storage.salt.buckets ：20，SALT的数量。当“tsd.storage.salt.width”为非0时，才发挥作用。该配置项的值，不能超过SALT的字节长度所能表示的最大数字。例如SALT的字节长度为1时，该配置项的不能配置成大于256。

- **tsd.storage.uid.width.metric**：3，metric UID的字节长度；
- **tsd.storage.uid.width.tagk**：3，tagk UID的字节长度；
- **tsd.storage.uid.width.tagv**：3，tagv UID的字节长度；
- **tsd.storage.use_max_value**：False，当TSDB压缩时发现重复的数据点是否采用更大数值的value（建档use_otsdb_timestamp设置为true时）；
- **tsd.storage.use_otsdb_timestamp**：True，使用数据点的时间戳而不是系统的现在时间；
- **tsd.storage.get_date_tiered_compaction_start**：0，Unix Epoch时间戳（ms），用于data tierd压缩；