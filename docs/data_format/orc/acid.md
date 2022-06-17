[toc]

# ACID

支持事务，**单次事务可以更新百万行**，但是不支持每小时百万的事务；

**HDFS是一次写的文件系统，ORC是一次写的文件格式，以 Base File + Delta File 形式实现**



使用ACID时，系统会增加delta目录：

| ILENAME                            | CONTENTS                           |
| :--------------------------------- | :--------------------------------- |
| 00000_0                            | Bucket 0 base                      |
| 00001_0                            | Bucket 1 base                      |
| delta_0000005_0000005/bucket_00000 | Transaction 5 to 5, bucket 0 delta |
| delta_0000005_0000005/bucket_00001 | Transaction 5 to 5, bucket 1 delta |

当太多的deltas目录时，minor compaction会自动运行并且将多个transaction合成单个delta；

| FILENAME                           | CONTENTS                            |
| :--------------------------------- | :---------------------------------- |
| 00000_0                            | Bucket 0 base                       |
| 00001_0                            | Bucket 1 base                       |
| delta_0000005_0000010/bucket_00000 | Transaction 5 to 10, bucket 0 delta |
| delta_0000005_0000010/bucket_00001 | Transaction 5 to 10, bucket 1 delta |

当deltas足够大时，major compaction会将deltas合并并重写base：

| FILENAME                  | CONTENTS                            |
| :------------------------ | :---------------------------------- |
| base_0000010/bucket_00000 | Transactions upto 10, bucket 0 base |
| base_0000010/bucket_00001 | Transactions upto 10, bucket 1 base |

读取和压缩不需要锁，因此压缩不能破坏性地修改它们的输入，而是**写入新的目录**。

为所有行提供一个自动分配的行id<原始事务id, bucket,  行id>，保证惟一。所有更新和删除操作都引用这个三元组。



ACID表中的ORC文件扩展为若干列。它们是操作(插入、更新或删除)、惟一标识行的三元组(原始事务、桶、rowId)和当前事务。

```c
struct<
  operation: int,
  originalTransaction: bigInt,
  bucket: int,
  rowId: bigInt,
  currentTransaction: bigInt,
  row: struct<...>
>
```

Operation的值为：

| OPERATION | SERIALIZATION |
| :-------- | :------------ |
| INSERT    | 0             |
| UPDATE    | 1             |
| DELETE    | 2             |

- 当应用程序或查询读取ACID表时，reader提供要包含的已提交事务列表。这个列表是在查询开始时由Hive metastore生成的。

- 该任务执行合并排序。每个文件按(初始事务升序、桶升序、rowId升序和当前事务降序)排序。

- 只返回要读取的事务列表中包含currentTransaction的第一个记录，它对应于对行的最后一个可见更新。



TODO: 流式ingest的扩展？



在ORC文件的元数据中添加了两个属性，以加速ACID表的处理。特别是，当一个任务正在读取一个bucket的基本文件的一部分时，它将使用第一个和最后一个rowIds来查找delta文件中的对应位置。

| KEY                 | MEANING                                                     |
| :------------------ | :---------------------------------------------------------- |
| hive.acid.stats     | Number of inserts, updates, and deletes comma separated     |
| hive.acid.key.index | The last originalTransaction, bucket, rowId for each stripe |