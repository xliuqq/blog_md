# [chunjun](https://github.com/DTStack/chunjun)

> A data integration framework.
>
> 原名FlinkX，是一款稳定、易用、高效、批流一体的数据集成框架，目前基于实时计算引擎**Flink**实现多种异构数据源之间的数据同步与计算

暂不支持Hudi（2022/12/8）

## 特性

将不同的数据库抽象成**reader/source 插件**，**writer/sink 插件**和**lookup 维表**插件

- 基于实时计算引擎Flink，支持JSON模版配置任务，兼容Flink SQL语法；
- 支持分布式运行，支持flink-standalone、yarn-session、yarn-per job等多种提交方式；
- 支持Docker一键部署，支持K8S 部署运行；
- 支持多种异构数据源，可支持MySQL、Oracle、SQLServer、Hive、Kudu等20多种数据源的同步与计算；
- 易拓展，高灵活性，新拓展的数据源插件可以与现有数据源插件即时互通，插件开发者不需要关心其他插件的代码逻辑；
- 不仅仅支持**全量同步**，还支持**增量同步**、间隔轮训；
- **批流一体**，不仅仅支持离线同步及计算，还兼容实时场景；
- 支持**脏数据**存储，并提供指标监控等；
- 配合checkpoint实现断点续传；
- 不仅仅支持同步DML数据，还支持Schema变更同步；



## 读写

