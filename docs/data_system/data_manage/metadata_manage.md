# 元数据管理

> （2024）Datahub都是目前最炙手可热的元数据管理平台。Openmetadata更有数据治理、数据资产管理平台的样子。

|                      | Apache Atlas       | Datahub（LinkedIn）                 | OpenMetadata       |
| -------------------- | ------------------ | ----------------------------------- | ------------------ |
| 技术栈               | Java               | Java + Python3.7                    | Java               |
| 数据发现             | 基于Hook机制       | 基于实时推送的自动发现              |                    |
| 支持数据种类         | 支持主流大数据组件 | 支持种类较多                        | 支持种类较多       |
| 支持RDB              |                    | ✅                                   |                    |
| 支持Hive             | ✅（有侵入性）      | ✅                                   | ✅                  |
| 支持数据湖           |                    | Iceberg                             | ✅                  |
| 数据分类             |                    | ✅（自定义标签）                     | ✅                  |
| 元数据检索（UI/API） | ✅                  | ✅                                   | ✅                  |
| 数据血缘和关系追踪   | ✅                  | ✅                                   | ✅                  |
| 安全性控制与隔离     | ✅（基于实体实例）  | ✅（基于用户和用户组等多种隔离机制） | ✅                  |
| 元数据质量分析       |                    | ✅                                   | ✅                  |
| 开源协议             | Apache-2.0 license | Apache-2.0 license                  | Apache-2.0 license |



## [Apache Atlas](http://atlas.apache.org/#/)

开源的大数据元数据管理和数据治理平台，旨在帮助组织收集、整理和管理数据的元数据信息。

它提供了丰富的元数据模型和搜索功能，可以与各种数据存储和处理平台集成。

- 支持 Hadoop 和非 Hadoop 元数据类型；
- 对数据血缘的追溯达到了字段级别；

技术架构：

- HBase 存储元数据，Solr实现索引；

问题：

- 社群活跃度一般，Atlas后期更新乏力；



## [Datahub](https://datahubproject.io/)

> Star 8.9K：The Metadata Platform for the Modern Data Stack.

LinkedIn开源的元数据搜索和发现平台。它提供了一个集中式的元数据存储库，用于管理和浏览各种类型的数据集和数据资产的元数据信息。

- 可以集成数据质量框架
- 搜索，数据血缘，数据分析，标签，术语表等功能
- 页面经过最新的改版，规划也较为合理，美观



## [OpenMetadata](https://github.com/open-metadata/OpenMetadata)

> Star 3.6K：Open Standard for Metadata. A Single place to Discover, Collaborate and Get your data right.

- UI 端支持多语言，UI非常美观，其操作和使用逻辑，也符合业务人员的习惯；
- 搜索，数据血缘，数据质量，数据分析，标签，术语表功能，并且有团队协作的功能。


