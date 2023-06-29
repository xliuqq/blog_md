# 协同编辑

多个用户对同一个文本域的同一个版本进行编辑时如何处理。

## 方案

### OT（Operation Transform）

OT的数据类型

https://github.com/ottypes

OT的问题：依赖于中心化的服务器



### CRDT（Conflict-Free Replicated Data Type）

CRDT(**Conflict-Free Replicated Data Type**)是各种**基础数据结构最终一致算法**的理论总结，能根据一定的规则自动合并，解决冲突，达到强最终一致的效果。

一个数据结构符合CRDT的条件是**update操作和merge操作**需满足**交换律**、**结合律**和**幂等律**。

- 如果update操作本身满足以上三律，merge操作仅需要对update操作进行回放即可，这种形式称为**op-based CRDT**，最简单的例子是集合求并集。
- 如果update操作无法满足条件，则可以考虑同步副本数据，同时附带额外元信息，通过元信息让update和merge操作具备以上三律，这种形式称为**state-based CRDT**。

CRDT 有以下几种常见的实现

**Counter**（计数器）：

- G-Counter(Grow-Only Set)：state-based CRDT
  - 只增不减的计数器，对于 N 个节点，每个节点上维护一个长度为 N 的向量 V；
  - update时只操作当前节点在数组中对应项即可；
  - merge时对数组每一项求max进行合并；
  - query时返回数组的和，即为counter的当前结果
- PN-Counter(Positive-Negative Counter)
  - 使用两个计数器来实现一个既能增加也能减少计数器；
  - 一个 G-Counter 来记录所有累加结果，另一个 G-Counter 来记录累减结果；

**Register**：本质山是个String，仅支持assign操作

- Last-Writer-Wins Register (LWW Register)
  - 后assign的覆盖先assign的(last write wins)，方式是每次修改都附带时间戳，update时通过时间戳生成偏序关系，merge时只取较大时间戳附带的结果

**Set**（集合）：add和remove（多节点并发进行add和remove操作是无法满足交换律）

- **G-Set(Grow-only Set)**：仅支持add，Op-based CRDT
  - add操作本质上是求并，天然满足交换律、结合律和幂等律；
- **2P-Set(Two-Phase Set)**：支持add/remove，Op-based CRDT
  - 使用两个G-Set，set A只负责添加，set R负责删除的元素；
  - 十分不实用，删除的元素不能再被添加，且删除的元素占用空间；
- **LWW-Element-Set(Last Write Wins element Set)**
  - 解决删除元素不能再次添加的问题，给2P-Set中A和R的每个元素加一个更新时间戳
- **OR-Set(Observed-Removed Set)**
  - 每次add(e)的时候都为元素e加一个唯一的tag，remove(e)将当前节点上的所有e和对应的tag都删除



## 工具

### ShareDB（OT）

> ShareDB is a full-stack library for realtime JSON document collaboration.

ShareDB is a realtime database backend based on [Operational Transformation (OT)](https://en.wikipedia.org/wiki/Operational_transformation) of JSON documents.

- It provides a **Node.js server** for coordinating and committing edits from multiple clients. 
- It also provides a **JavaScript client** for manipulating documents, which can be run either in Node.js or in the browser.

### Yjs（CRDT)

> Yjs is a modular framework for syncing things in real-time - like editors!

#### YModel

Yjs 对使用者提供了如 YText、YArray 和 YMap 等常用数据类型，即所谓的 Shared Types；

YModel 实例中的数据都存储在一条双向链表里。粗略地说，**这条链表中的每一项（或者说每个 item）都唯一地记录了某次用户操作所修改的数据**，对 YModel 的操作，最后都会转为对这条链表的 append、insert、split、merge 等结构变换。

> 基于上述手段实现的 CRDT 结构是 CRDT 流派中的一种，可以将其称为 operation-based CRDT、list CRDT 或 sequence CRDT。

每个 item 分配一个唯一 ID(唯一确定其在逻辑时间轴上的位置)，其结构为 `ID(clientID, clock)`

- clientID：唯一标识某个 YDoc 对应的客户端；
- clock：Lamport timestamp 的逻辑时间戳



#### 原理

- 基于双向链表和 StructStore 的基础数据结构
- 基于 YATA 算法的并发冲突解决机制
- 基于 DeleteSet 和 Transaction 机制的撤销重做
- 基于两阶段划分的同步机制

- 文字内容的快速插入：https://josephg.com/blog/crdts-go-brrr/



### Diamond-Types

https://github.com/josephg/diamond-types

## 参考文献：

[shareDB作者 CRDTs are the future](https://josephg.com/blog/crdts-are-the-future/)

