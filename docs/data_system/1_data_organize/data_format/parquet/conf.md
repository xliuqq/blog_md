[toc]

# 配置

## Overall

### Row Group size

Row group越大，column chunks越大，可以支持更大的顺序IO。但是写的时候需要更多的buffer或者两次写。推荐大小为512M~1G。

Row group大小和HDFS block配置一样最好。

An optimized read setup would be: **1GB row groups, 1GB HDFS block size, 1 HDFS block per HDFS file.**

### Data Page size

Data page是不可分割的，小的data page允许更细粒度的读（如单行查找），大的page size会降低空间和解析开销。推荐8KB。



## Hadoop

要使用InputFormat或OutputFormat，需要实现WriteSupport或ReadSupport类，它们将实现对象与Parquet的转换。

Parquet项目已经内置了Thrift、Avro和Protobuf的实现。

https://github.com/apache/parquet-mr/tree/master/parquet-hadoop#class-parquetoutputformat

### ParquetOutputFormat

#### parquet.summary.metadata.level

**Description** ： 在与parquet文件相同的目录中写入summary 文件。

- *all*
- *common_only*
- *none*

**Default value** ： `all`

------

#### parquet.enable.summary-metadata

**Description**: Deprecated, use `parquet.summary.metadata.level` instead.

- *true*：等价于`parquet.summary.metadata.level` 值为 *all*；

- *false*：等价于`parquet.summary.metadata.level` 值为 *none*；

**Default value** ： `true`

------

#### parquet.block.size

**Description**: Block 大小（bytes）

- block_size = max(default_fs_block_size, parquet.block.size)

- row_group_size = `parquet.block.size`

**Default value** ： `134217728` (128 MB)

------

#### parquet.page.size

**Description** ：page size in bytes is for compression. 每个Page可以独立解压缩。

- 页是必须完全读取才能访问单个记录的最小单元。值不宜过小。

**Default value** ： `1048576` (1 MB)

------

#### parquet.compression

**Description**：压缩page的算法，替代`mapred.output.compresss*`

- 值可以是`uncompressed`, `snappy`, `gzip` or `lzo`, `brotli`,`lz4` and `zstd`；
- 如果值未设置，`FileOutputFormat`会使用以下参数
  - `mapreduce.output.fileoutputformat.compress`
  - `mapreduce.output.fileoutputformat.compress.codec`

**Default value** ： `uncompressed`

------

#### parquet.write.support.class

**Description** ：write suuport 类，必须是`org.apache.parquet.hadoop.api.WriteSupport`子类。

------

#### parquet.enable.dictionary

**Description**：是否启动词典编码，*true* 或 *false*

- 可以为单独的列指定是否启动词典编码

  ```java
  // Enable dictionary encoding for all columns
  conf.set("parquet.enable.dictionary", true);
  // Disable dictionary encoding for 'column.path'
  conf.set("parquet.enable.dictionary#column.path", false);
  // The final configuration will be: Enable dictionary encoding for all columns except 'column.path'
  ```

**Default value** ： `true`

------

#### parquet.dictionary.page.size

**Description**：词典页的大小。

- 当启动词典编码时，每个row group中每列都有一个词典页。

**Default value** ： `1048576` (1 MB)

------

#### parquet.validation

**Description**：是否使用schema验证

**Default value** ： `false`

------

#### parquet.writer.version

**Description**：`PARQUET_1_0` or `PARQUET_2_0`

- `PARQUET_1_0` ：DataPageHeaderV1
- `PARQUET_2_0` ：DataPageHeaderV2

**Default value** ： `PARQUET_1_0`

------

#### parquet.memory.pool.ratio

**Description**：内存管理器通过该参数均衡每个Parquet writer的分配大小，0到1。

- 如果所有writer的分配大小超过总和（total memory pool），则对每个writer的分配大小乘以该值。

**Default value** ： `0.95`

------

#### parquet.memory.min.chunk.size

**Description**：每个Parquet writer的最小分配值。

- 如果分配值小于该参数，则memory manager抛出异常。 

**Default value** ： `1048576` (1 MB)

------

#### parquet.writer.max-padding

**Description**：为了对齐row group的最大填充的字节大小。

**Default value** ： `8388608` (8 MB)

------

#### parquet.page.size.row.check.min/max

**Description**：检查页面大小限制的频率，介于`parquet.page.size.row.check.min` 和`parquet.page.size.row.check.max`之间。

- 频率高，则page size更精确；
- 频率低，则性能更好。

**Default value** ： `10000`

------

#### parquet.page.size.check.estimate

**Description**：如果为`true`，则colum writer估计下一页的大小。

**Default value** ： `true`

------

#### parquet.columnindex.truncate.length

**Description**：列索引包含列块中页的最小/最大和空计数值

- 如果可能的话，此属性是用于截断二进制列索引中的二进制值的长度。

**Default value** ： `64`

------

#### parquet.statistics.truncate.length

**Description**：行组中的最小/最大二进制值试图截断为的长度。

**Default value** ： `2147483647`

------

#### parquet.bloom.filter.enabled

**Description**：是否采用bloom过滤器

- 可以单独指定某些列启用bloom过滤器，通过`#`。

```java
conf.set("parquet.bloom.filter.enabled#column.path", false);
```

**Default value** ： `false`

------

#### parquet.bloom.filter.expected.ndv

**Description**：列中不同值的期望值，用于计算Bloom过滤器的最佳长度。

```java
// The bloom filter will be enabled for 'column.path' with expected number of distinct values equals to 200
conf.set("parquet.bloom.filter.expected.ndv#column.path", 200)
```

**Default value** ： `parquet.bloom.filter.max.bytes`

------

#### parquet.bloom.filter.max.bytes

**Description**：布隆过滤器bitset的最大字节数

**Default value** ： `1048576` (1MB)

------

#### parquet.page.row.count.limit

**Description**：每页中最大的行的个数

**Default value** ： `20000`

------

#### parquet.page.write-checksum.enabled

**Description**：是否写页的校验和

**Default value** ： `true`

------

#### parquet.crypto.factory.class

**Description**： Class implementing EncryptionPropertiesFactory

**Default value** ：  None



### ParqauetInputFormat

#### parquet.read.support.class

**Description**：ParquetInputFormat使用的read support 类进行物化记录（materialize records）。

- `org.apache.parquet.hadoop.api.ReadSupport` 子类

------

#### parquet.read.filter

**Description**：`org.apache.parquet.filter.UnboundRecordFilter`的子类，旧的filter API。

------

#### parquet.private.read.filter.predicate

**Description**：新的filter API，继承 `org.apache.parquet.filter2.FilterPredicate`

- 该属性值：先对java序列化的对象进行gzip压缩，然后base64编码成byte数组，再构造为UTF-8 String；

- 新的filter API可以过滤记录或过滤记录的整行组，而不需要读取它们。
- 新和旧的filter API只能选其一；
- 通过`ParquetInputFormat.setFilterPredicate`手动设置filter类

------

#### parquet.strict.typing

**Description**：是否为有冲突的schema启用类型检查。

**Default value** ： `true`

------

#### parquet.filter.record-level.enabled

**Description**：是否启用记录级筛选。

**Default value** ： `true`

------

#### parquet.filter.stats.enabled

**Description** ：是否启用行组统计信息筛选。

**Default value** ： `true`

------

#### parquet.filter.dictionary.enabled

**Description** ：是否启用行组字典筛选。

**Default value** ： `true`

------

#### parquet.filter.columnindex.enabled

**Description** ：是否启用页的列索引筛选。

**Default value** ： `true`

------

#### parquet.page.verify-checksum.enabled

**Description** ：是否启用页级校验和验证。

**Default value** ： `false`

------

#### parquet.filter.bloom.enabled

**Description** ：是否启用行组Bloom过滤。

**Default value** ： `true`

------

#### parquet.task.side.metadata

**Description**： 是否打开或关闭任务端元数据加载:

- `true` ：在任务端读取元数据，一些任务可能会立即完成。
- `false` ：元数据在客户端读取，如果有大量元数据，读取速度会慢一些。但只有在有工作要做时，任务才会衍生出来。

**Default value** ： `true`

------

#### parquet.split.files

**Description**：如果为false，则按顺序读取文件。

**Default value** ： `true`



### ReadSupport

#### parquet.read.schema

**Description** ：读取投影的schema。

- 用于映射下推，只读取需要的列。



### UnmaterializableRecordCounter

#### parquet.read.bad.record.threshold

**Description** ：可以容忍的坏记录的百分比

**Default value** ： `0`



### ZstandardCodec

#### parquet.compression.codec.zstd.level

**Description**  ZSTD压缩级别，1~22，值越高压缩比越高，但写时间越长

**Default value** ： `3`

------

#### parquet.compression.codec.zstd.workers

**Description** ：并行压缩的线程数。值为0表示单线程

**Default value** ： `0`



### HadoopReadOptions

#### parquet.crypto.factory.class

**Description** ：实现`DecryptionPropertiesFactory`的类

**Default value** ： None



### PropertiesDrivenCryptoFactory

#### parquet.encryption.column.keys

**Description** ：加密的列的列表，见 HIVE-21848

- 格式： `<masterKeyID>:<colName>,<colName>;<masterKeyID>:<colName>...`

**Default value** ： None. 

------

#### parquet.encryption.footer.key

**Description** ：Master key ID for footer encryption/signing.

**Default value** ： None. If neither `column.keys` nor `footer.key` are set, the file won't be encrypted by the PropertiesDrivenCryptoFactory. If one of the two properties is set, an exception will be thrown.

------

#### parquet.encryption.algorithm

**Description** ：AES_GCM_V1` or `AES_GCM_CTR_V1`.

**Default value** ： `AES_GCM_V1`

------

#### parquet.encryption.plaintext.footer

**Description** ：Write files in plaintext footer mode, that makes many footer fields visible (e.g. schema) but allows legacy readers to access unencrypted columns. The plaintext footer is signed with the footer key. If `false`, write files in encrypted footer mode, that fully encrypts the footer, and signs it with the footer key.
**Default value** ： `false`

------

#### parquet.encryption.kms.client.class

**Description** ：Class implementing the KmsClient interface. "KMS" stands for “key management service”. The Client will interact with a KMS Server to wrap/unrwap encryption keys.

**Default value** ： None

------

#### parquet.encryption.kms.instance.id

**Description** ：ID of the KMS instance that will be used for encryption (if multiple KMS instances are available).

**Default value** ： `DEFAULT`

------

#### parquet.encryption.kms.instance.url

**Description** ：URL of the KMS instance.

**Default value** ： `DEFAULT`

------

#### parquet.encryption.key.access.token

**Description** ：Authorization token that will be passed to KMS.

**Default value** ： `DEFAULT`

------

#### parquet.encryption.double.wrapping

**Description** ：Use double wrapping - where data encryption keys (DEKs) are encrypted with key encryption keys (KEKs), which in turn are encrypted with master keys. If `false`, DEKs are directly encrypted with master keys, KEKs are not used.

**Default value** ： `true`

------

#### parquet.encryption.cache.lifetime.seconds

**Description** ：Lifetime of cached entities (key encryption keys, local wrapping keys, KMS client objects).

**Default value** ： `600` (10 minutes)

------

#### parquet.encryption.wrap.locally

**Description** ：Wrap keys locally - master keys are fetched from the KMS server and used to encrypt other keys (DEKs or KEKs). If `false` - key wrapping will be performed by a KMS server.

**Default value** ： `false`

------

#### parquet.encryption.key.material.store.internally

**Description** ：Store key material inside Parquet file footers; this mode doesn’t produce additional files. If `false`, key material is stored in separate new files, created in the same folder - this mode enables key rotation for immutable Parquet files.

**Default value** ： `true`

------

#### parquet.encryption.data.key.length.bits

**Description** ：Length of data encryption keys (DEKs), randomly generated by parquet key management tools. Can be 128, 192 or 256 bits.

**Default value** ： `128`