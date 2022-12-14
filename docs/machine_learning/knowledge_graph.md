# 知识图谱

> 知识图谱这个概念最早由Google提出，主要是用来优化现有的搜索引擎。

知识图谱本质上是**语义网络**，是一种**基于图的数据结构**，由**节点(Point)和边(Edge)**组成。

在知识图谱里，每个节点表示现实世界中存在的**“实体”**，每条边为实体与实体之间的**“关系”**。



**特定领域里的 (Domain-Specific) 知识图谱表示方式和应用**



存储方式主要有两种形式：**RDF存储格式（如Apache Jena）** 和 **图数据库（如 Neo4j）**。



## 应用场景

### 反欺诈

反欺诈的核心是人，首先需要把与借款人相关的所有的数据源打通，并构建包含多数据源的知识图谱。

整合借款人的基本信息（比如申请时填写的信息），还可以把借款人的消费记录、行为记录、网上的浏览记录等。

- **不一致性验证（知识推理）**：借款人填写同一个公司电话，但是公司名不一样；
- **组团欺诈**：借款人没有直接关系，但是共享部分信息；
- **异常分析**：静态分析（异常结构），动态分析（分析其结构随时间变化的趋势）；