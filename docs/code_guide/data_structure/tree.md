# 树结构

## 平衡二叉树

### AVL树

### 红黑树



## B/B+/B*树

> 查找磁盘的过程中减少IO的次数。每个节点对应一个Page，数据以Page为单位在内存和磁盘间进行调度。

### B树(B-tree)

> 注意:之前有看到有很多文章把B树和B-tree理解成了两种不同类别的树，其实这两个是同一种树;

平衡多路查找树，**m阶的B树**满足下列条件：

- 树中每个结点至多有m个孩子；
- 除根结点和叶子结点外，其它每个结点至少有m/2个孩子；
- 若根结点不是叶子结点，则至少有2个孩子；
- 所有叶子结点(失败节点)都出现在同一层，叶子结点不包含任何关键字信息；
- 非叶子结点的指针：P[1], P[2], …, P[M]；其中P[1]指向关键字小于K[1]的子树，P[M]指向关键字大于K[M-1]的子树，其它P[i]指向关键字属于(K[i-1], K[i])的子树；

B树**每个节点都存储数据**，所有节点组成这棵树。

![img](pics/b.png)

### B+树

B+树只有**叶子节点存储数据**。

所有**关键字都出现在叶子结点的链表**中（稠密索引），且链表中的关键字是有序的。

**非叶子结点的子树指针P[i]，指向关键字值属于 [K[i], K[i+1]) 的子树**（B-树是开区间）；

<font color='red'>更适合文件索引系统？</font>

![b+](./pics/b+.png)

- B+**树的层级更少**：相较于B树B+每个**非叶子**节点存储的关键字数更多，树的层级更少所以查询数据更快；
- B+**树查询速度更稳定**：B+所有关键字数据地址都存在**叶子**节点上，所以每次查找的次数都相同所以查询速度要比B树更稳定;
- B+**树天然具备排序功能：**B+树所有的**叶子**节点数据构成了一个有序链表，在查询大小区间的数据时候更方便，数据紧密性很高，缓存的命中率也会比B树高。
- B+**树全节点遍历更快：**B+树遍历整棵树只需要遍历所有的**叶子**节点即可，而不需要像B树一样需要对每一层进行遍历，这有利于数据库做全表扫描。



### B*树

B*树定义了非叶子结点关键字个数至少为(2/3)*M，即块的最低使用率为2/3（代替B+树的1/2）；

在B+树的非根和非叶子结点再增加指向兄弟的指针；

- B+树节点满时就会分裂，而B*树节点满时会检查兄弟节点是否满（因为每个节点都有指向兄弟的指针），如果兄弟节点未满则向兄弟节点转移关键。

![b_star](pics/b_star.png)

在B+树的基础上因其初始化的容量变大，使得节点空间使用率更高，而又存有兄弟节点的指针，可以向兄弟节点转移关键字的特性使得B*树额外分解次数变得更少；



### LSM树

- 在 B+
- 树 基础上，将读写分离，**读操作先内存后磁盘、数据写操作（包括插入、修改、删除也是写）都在内存中**进行。到达**一定阈值的时候才会刷新到磁盘**上。在大规模情况下，寻道明显比传输低效。
- LSM树原理把一棵大树拆分成N棵小树，它首先写入内存中，随着小树越来越大，内存中的小树会flush到磁盘中，**磁盘中的树定期可以做merge操作**，合并成一棵大树，以优化读性能。
- LSM树（Log-Structured Merge Tree）存储引擎和B树存储引擎一样，同样支持增、删、读、改、顺序扫描操作。而且通过批量存储技术**规避磁盘随机写入**问题。当然凡事有利有弊，LSM树和B+树相比，LSM树牺牲了部分读性能，用来大幅提高写性能。

#### 内存

可以使用平衡二叉树（如红黑树）、跳表（skiplist）等维护有序的数据结构，考虑道并发性能，HBase使用跳表。

#### 磁盘

多个内部KeyValue有序的文件组成

#### Compaction

compaction的两个极端：

- 第一种极端（左上log方案），根本不做compaction，所有的数据全部写到Log然后灌入SStable，
  - update cost最低，不需compaction，直接Disk；
  - 查询一个K/V，线性查，效率非常低，且没有compaction的话数据会随着写入线性增长；
- 第二种极端（右下sorted array方案）, 每次写新的，立马就去做compaction，永远维持整个数据更新到最新值并且sorted状态，
  - 查询固然效率最高，直接按照index就行；
  - 每次写入整个数据都去调整、sort，update的花销太大。写入KPS一高，机器撑不住。

![lsm_compaction.png](pics/lsm_compaction.png)

##### tiering compaction

<img src="pics/lsm_tiering.webp" alt="img" style="zoom: 80%;" />

- 每层允许的SST文件最大数量都有个相同的阈值；
- memtables 首先会不停flush 到第一层很小的sstables，等到sstable数量够了（图里4个），compaction成一个sstable写到下一层，下一层sstable写满4个，再compact，如此以往；
- 能够保证每个sstable是sorted，但是同一层 sstable 的 key 会互相覆盖（*overlapping*） 

##### Level compaction

在Disk 里面维持了多级level的SStable，而且每层维持“**唯一一个**”Run。

> Each run contains data ***sorted*** by the index key. A run can be represented on disk as a single file, or alternatively as a collection of files with ***non-overlapping*** key ranges.

compaction的过程可以简化成：**in memory的table写满了，那么就flush到第一级的Disk SStable并且做compaction，要是第一级又满了，就又开始flush到第二级做compaction，以此类推直到max level**

![img](pics/lsm_leveling_compaction.webp)

- **写满**的定义：sstables的大小基本固定，限制每一层的sstables的个数，超过阈值后，选择一个sstable与下一层进行合并（L1的一个sstable大致对应L2的10个sstable)；



**tiering跟leveling的compaction的对比：**

![lsm_compaction_compare.png](pics/lsm_compaction_compare.png)

|                     | 优点                                                         | 缺点                                                         |
| ------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| tiering compaction  | 简单且易于实现；<br />SST数目少，定位到文件的速度快<br />写放大较小<br /> | 需要查询多个文件<br />单个SST的大小有可能会很大，空间放大严重<br />读放大严重 |
| leveling compaction | 空间放大较小<br />读放大较小                                 | 写放大严重                                                   |

**空间放大**（space amplification）:

- 指存储引擎中的数据实际占用的磁盘空间比数据的真正大小偏多的情况；

  - 例如，数据的真正大小是10MB，但实际存储时耗掉了25MB空间，那么空间放大因子（space amplification factor）就是2.5。

- tiering compaction 因为大的SSTable，需要更多的临时空间去合并；

- leveling compaction，不必再选取一层内所有的数据，并且每层中SST的key区间都是不相交的

**写放大（write amplification）**

- 指实际写入磁盘的记录数跟写入记录的比例，即同一个记录需要被多次写入Disk；
- 耗费磁盘带宽；存储介质的寿命降低；
- leveling compaction写放大严重：Ln层SST在合并到Ln+1层时是一对多的，故重复写入的次数会更多；

**读放大**

每次读请求带来的读盘次数



##### RocksDB的混合compaction策略（leveled compaction）

当L0层的文件数目达到`level0_file_num_compaction_trigger`阈值时，就会触发L0层SST合并到L1。

<img src="pics/lsm_leveld_1.webp" alt="img" style="zoom: 50%;" />

L1层及以后的compaction过程完全符合前文所述的leveled compaction逻辑，如下图所示，很容易理解。

<img src="pics/lsm_leveld_2.webp" alt="img" style="zoom:50%;" />

<img src="pics/lsm_leveld_3.webp" alt="img" style="zoom:50%;" />

<img src="pics/lsm_leveld_4.webp" alt="img" style="zoom: 50%;" />

<img src="pics/lsm_leveld_5.webp" alt="img" style="zoom:50%;" />

多个compaction过程是可以并行进行。




### Fractal Tree(分形树)

索引结构来解决随机IO的问题，能让随机IO变成顺序IO。



## 跳表

> 高效实现插入、删除、查找的内存数据结构。复杂度期望为O(logN)

- 实现简单，并发场景下加锁的粒度更小；（HBase、Redis、LevelDB）

### 增

### 删

### 改/查