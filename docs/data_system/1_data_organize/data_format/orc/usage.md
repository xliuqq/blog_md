[toc]

# 使用

## Java

### Vectorized Row Batch

数据作为VectorizedRowBatch的实例传递给ORC，该实例包含1024行数据。



| array     | [ListColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/ListColumnVector.html) |
| --------- | ------------------------------------------------------------ |
| binary    | [BytesColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/BytesColumnVector.html) |
| bigint    | [LongColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/LongColumnVector.html) |
| boolean   | [LongColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/LongColumnVector.html) |
| char      | [BytesColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/BytesColumnVector.html) |
| date      | [LongColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/LongColumnVector.html) |
| decimal   | [DecimalColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/DecimalColumnVector.html) |
| double    | [DoubleColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/DoubleColumnVector.html) |
| float     | [DoubleColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/DoubleColumnVector.html) |
| int       | [LongColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/LongColumnVector.html) |
| map       | [MapColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/MapColumnVector.html) |
| smallint  | [LongColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/LongColumnVector.html) |
| string    | [BytesColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/BytesColumnVector.html) |
| struct    | [StructColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/StructColumnVector.html) |
| timestamp | [TimestampColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/TimestampColumnVector.html) |
| tinyint   | [LongColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/LongColumnVector.html) |
| uniontype | [UnionColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/UnionColumnVector.html) |
| varchar   | [BytesColumnVector](https://orc.apache.org/api/hive-storage-api/index.html?org/apache/hadoop/hive/ql/exec/vector/BytesColumnVector.html) |



BytesColumnVector中每个数据表示为字节数组、偏移量和长度。值之间可以共享字节数组，也可以不共享。

```java
public class BytesColumnVector extends ColumnVector {
  public byte[][] vector;
  public int[] start;
  public int[] length;
  ...
}
```



### 写示例

```java
Path testFilePath = new Path("advanced-example.orc");
Configuration conf = new Configuration();

TypeDescription schema = TypeDescription.fromString("struct<first:int," +
                               "second:int,third:map<string,int>>");

Writer writer = OrcFile.createWriter(testFilePath,
        OrcFile.writerOptions(conf).setSchema(schema));

VectorizedRowBatch batch = schema.createRowBatch();
LongColumnVector first = (LongColumnVector) batch.cols[0];
LongColumnVector second = (LongColumnVector) batch.cols[1];

//Define map. You need also to cast the key and value vectors
MapColumnVector map = (MapColumnVector) batch.cols[2];
BytesColumnVector mapKey = (BytesColumnVector) map.keys;
LongColumnVector mapValue = (LongColumnVector) map.values;

// Each map has 5 elements
final int MAP_SIZE = 5;
final int BATCH_SIZE = batch.getMaxSize();

// Ensure the map is big enough
mapKey.ensureSize(BATCH_SIZE * MAP_SIZE, false);
mapValue.ensureSize(BATCH_SIZE * MAP_SIZE, false);

// add 1500 rows to file
for(int r=0; r < 1500; ++r) {
  int row = batch.size++;

  first.vector[row] = r;
  second.vector[row] = r * 3;

  map.offsets[row] = map.childCount;
  map.lengths[row] = MAP_SIZE;
  map.childCount += MAP_SIZE;

  for (int mapElem = (int) map.offsets[row]; mapElem < map.offsets[row] + MAP_SIZE; ++mapElem) {
    String key = "row " + r + "." + (mapElem - map.offsets[row]);
    mapKey.setVal(mapElem, key.getBytes(StandardCharsets.UTF_8));
    mapValue.vector[mapElem] = mapElem;
  }
  if (row == BATCH_SIZE - 1) {
    writer.addRowBatch(batch);
    batch.reset();
  }
}
if (batch.size != 0) {
  writer.addRowBatch(batch);
  batch.reset();
}
writer.close();
```



### 读示例

```java
Reader reader = OrcFile.createReader(new Path("my-file.orc"),
                  OrcFile.readerOptions(conf));

// 默认 RecordReader 读取所有的行和列
RecordReader rows = reader.rows();
VectorizedRowBatch batch = reader.getSchema().createRowBatch();

while (rows.nextBatch(batch)) {
  for(int r=0; r < batch.size; ++r) {
    ... process row r from batch
  }
}
rows.close();
```