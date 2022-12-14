# OpenLooKong

> **华为**开源，早期源自*Presto*构建基础的交互式查询能力

功能：

- 支持数据探索、即席查询和批处理，具有100+毫秒至分钟级的近实时时延，而无需移动数据；
- 具有高可用性、自动伸缩、内置缓存和索引支持，为企业工作负载提供了所需的可靠性；

适用于：

- **跨源异构查询场景**：RDBMS、NoSQL、Hive、MPPDB等数据仓库的联合查询

- **跨域跨DC查询**：openLooKeng集群部署在多个DC中；

- **存储计算分离**：本身没有存储引擎，但可以查询存储在不同数据源中的数据；

- **快速数据探索**：使用标准SQL定义一个虚拟数据市场，通过跨源异构查询能力连接各个数据源

