# 编码

## 基本类型编码

Parquet以列存储，因此编码是针对一列（相同类型的数组）进行

Parquet格式的Thrift定义：https://github.com/apache/parquet-format/blob/master/src/main/thrift/parquet.thrift 

### <a name="PLAIN">Plain(PLAIN = 0)</a>

支持所有数据类型，对数据没有压缩和其他处理。当数据类型不能用更有效的编码时采用PLAIN。

- Boolean：Bit Packed, LSB first
- INT32: 4 bytes little endian
- INT64: 8 bytes little endian
- INT96: 12 bytes little endian (deprecated)
- FLOAT: 4 bytes IEEE little endian
- DOUBLE: 8 bytes IEEE little endian
- BYTE_ARRAY: length in 4 bytes little endian followed by the bytes contained in the array
- FIXED_LEN_BYTE_ARRAY: the bytes contained in the array

### Dictionary Encoding (PLAIN_DICTIONARY = 2 and RLE_DICTIONARY = 8)

构建一个包含**给定列中遇到的值的字典**。字典将存储在**每个列块的字典页**中。

将值存储为整数并使用[RLE/Bit-Packing](#RLE)混合编码。如果字典变得太大，无论是在大小上还是在不同值的数量上，都会退回到PLAIN编码。**在列块的数据页之前，首先写入字典页**。

- 词典页格式：the entries in the dictionary - in dictionary order，使用 [plain](#PLAIN) 编码.
- 数据页格式：1byte表示bitwidth(最大32)，值用RLE/bit packed编码；

推荐：Parquet 2.0+ 数据页使用 RLE_DICTIONARY，词典页使用 PLAIN 。

### <a name="RLE">Run Length Encoding / Bit-Packing Hybrid (RLE = 3)</a>

针对重复（repeated）的数据，使用**big-packing**和**run-lenght-encoding**组合编码。

RLE编码支持的数据类型：

- `Repetition, definition level`
- `Dictionary index`
- `Boolean values in data pages, as an alternative to PLAIN encoding`

```properties
# 编码的组成
rle-bit-packed-hybrid: <length> <encoded-data>
length := length of the <encoded-data> in bytes stored as 4 bytes little endian (unsigned int32)
encoded-data := <run>*

run := <bit-packed-run> | <rle-run>
bit-packed-run := <bit-packed-header> <bit-packed-values>
bit-packed-header := varint-encode(<bit-pack-scaled-run-len> << 1 | 1)
// we always bit-pack a multiple of 8 values at a time, so we only store the number of values / 8
bit-pack-scaled-run-len := (bit-packed-run-len) / 8
bit-packed-run-len := *see 3 below*
bit-packed-values := *see 1 below*

rle-run := <rle-header> <repeated-value>
rle-header := varint-encode( (rle-run-len) << 1)
rle-run-len := *see 3 below*
repeated-value := value that is repeated, using a fixed-width of round-up-to-next-byte(bit-width)
```

1. 这里的*bit-packing* 跟 [deprecated bit-packing](#BITP) 使用不同的顺序。这些值从每个字节的**最低有效位打包到最高有效位**，但是**每个值中的位的顺序仍然按照通常的最高有效位到最低有效位的顺序**。

   示例：1-7的数字

   ```properties
   dec value: 0   1   2   3   4   5   6   7
   bit value: 000 001 010 011 100 101 110 111
   bit label: ABC DEF GHI JKL MNO PQR STU VWX
   ==>
   bit value: 10001000 11000110 11111010
   bit label: HIDEFABC RMNOJKLG VWXSTUPQ
   ```

   这种打包顺序的原因是在一次反序列化多个字节时，在little-endian硬件上有更少的字边界。这是因为可以将4个字节读入32位寄存器(或将8个字节读入64位寄存器)，并且可以通过**shift和ORing with mask**来解压缩值。

2. varint-encode() 使用 ULEB-128 encoding；

3. `bit-packed-run-len`和`rle-run-len`要在$[1, 2^{31} - 1$] 中。



### <a name="BITP">Bit-packed (Deprecated) (BIT_PACKED = 4)</a>

仅**bit-packed**编码，被 [RLE/bit-packing](#RLE)编码替代。每个值用固定的bit位进行编码，值之间没有填充(最后一个字节除外，用0来填充）。

出于兼容性的原因，实现将值从**最高有效位打包到最低有效位**。

**只被用来编码*definition & repetition level*。**

**示例**：1-7数字使用3bit长度的编码

```properties
dec value: 0   1   2   3   4   5   6   7
bit value: 000 001 010 011 100 101 110 111
bit label: ABC DEF GHI JKL MNO PQR STU VWX
==> 
bit value: 00000101 00111001 01110111
bit label: ABCDEFGH IJKLMNOP QRSTUVWX
```

### <a name="DELE">Delta Encoding (DELTA_BINARY_PACKED = 5)</a>

支持的数据类型：INT32, INT64

*ZigZag ULEB128 int* 表示先对 int 进行ZigZag编码，再进行ULEB128编码。

```properties
delta encoding: <header><blocks of delta encoded values>

header:= <block size in values> <number of miniblocks in a block> <total value count> <first value>
block size in values:= <ULEB128 int> 
number of miniblocks in a block:= <ULEB128 int>
total value count:= <ULEB128 int>
first value:= <ZigZag ULEB128 int>

blocks of delta encoded values:= <block>*
block:= <min delta> <list of bitwidths of miniblocks> <miniblocks>
min delta:= <ZigZag ULEB128 int>
list of bitwidths of miniblocks:= <byte>*
miniblocks:= <bit width><bit packed ints>*
```

- *block size in values*：block的大小，必须是128的倍数，用ULEB 128 int编码；
- *number of miniblocks in a block*：block中的miniblock的个数，用block size除以它可以算出每个miniblock的元素个数（必须是32的倍数），用ULEB 128 int编码；

**block的编码**

- 计算连续值的差值（delta），第一个块中第一个值在header中存储，其它块的第一个值跟其上一块的最后一个值，计算差值；
- 计算 the frame of reference（块中delta的最小值)。从所有的deltas中减去min delta。这保证了所有的值都是非负；
- 将min delata采用zigzag ULEB128 int编码，每个miniblock中的bitwidth采用byte编码，每个miniblock中的delta values（减去min delta）采用bit packed编码；

如果没有足够的值填满最后一个miniblock，采用0填充这个miniblock，使其长度就等于整个miniblock中值的数量乘以位宽（bitwidth）。

如果最后一个block中miniblock个数少于`<number of miniblocks in a block`，存储无需的miniblock的bitwidth仍然需要（bitwodth为0），但是无需的miniblock的数据不需要填充，因为读取器可以根据已读字节数进行停止读取。

**示例**

```properties
# use 8 as the block size
# original data
7, 5, 3, 1, 2, 3, 4, 5
# the deltas would be
-2, -2, -2, 1, 1, 1, 1
# The minimum is -2, so the relative deltas are:
0, 0, 0, 3, 3, 3, 3
# The encoded data is
header: 8 (block size), 1 (miniblock count), 8 (value count), 7 (first value)

blocks(only 1 block): -2 (minimum delta), 2 (bitwidth), 00000011111111b (0,0,0,3,3,3,3 packed on 2 bits)
```



### Delta-length byte array: (DELTA_LENGTH_BYTE_ARRAY = 6)

BYTE_ARRAY 数据类型。

**对于字节数组列，这种编码总是优于PLAIN编码。**

先编码所所有byte array的长度（使用**DELTA_BINARY_PACKED**），再拼接上所有的byte array data。通过长度编码和更好的数据压缩（不再与长度交叉）。

示例：

```
"Hello", "World", "Foobar", "ABCDEF"
==>
DeltaEncoding(5, 5, 6, 6) "HelloWorldFoobarABCDEF"
```



### Delta Strings: (DELTA_BYTE_ARRAY = 7)

BYTE_ARRAY 数据类型。

也成为增量编码或者前向压缩。**对于字符串序列中的每个元素，存储前一个条目的前缀长度加上后缀。**

值存储为 前缀序列 + 后缀值

- Prefix Length序列：采用 **DELTA_BINARY_PACKED** 编码；
- Suffixes：采用 **DELTA_LENGTH_BYTE_ARRAY** 编码；



### Byte Stream Split: (BYTE_STREAM_SPLIT = 9)

FLOAT DOUBLE数据类型。

这种编码不会减少数据的大小，但在使用压缩算法后，可以显著提高压缩比和速度。

创建了**长度为N的K个字节流**，其中K是数据类型的字节大小，N是数据序列中元素的数量。**每个值的字节被分散到相应的流中。**第0个字节进入第0个流，第1个字节进入第1个流，依此类推。这些流按以下顺序连接起来：第0个流、第1个流，等等。

示例：原始数据是三个32位浮点数（K=4, N=3）。

```
       Element 0      Element 1      Element 2
Bytes  AA BB CC DD    00 11 22 33    A3 B4 C5 D6
=>
Bytes  AA 00 A3    BB 11 B4    CC 22 C5   DD 33 D6
```



## 列嵌套编码

数据结构，其根为Record，称为message，其内部每个字段包含三部分：**字段类型、数据类型、字段名称**。通过**数据类型group来表达层级关系**。

- required：exactly one occurrence
- optional: 0 or 1 occurrence
- repeated: 0 or more occurrence

*required*, *optional*, *repeated*：针对optional和repeated，列示存储下，无法确定属于行的边界

编码嵌套列，采用 *Dremel encoding with definition and repetition levels*。

- **Definition level**
  - 指定在**列的路径中定义了多少个可选字段**
- **Repetition level**
  - 指定**路径中哪些重复字段的值是重复**的

最大的Definition和Repetition层次是可以通过schema计算出来。这定义了存储级别所需的最大位数(级别是为列中的所有值定义的)。

### 示例

```thift
message Record {
	required string sid, 
	repeated long appid,
	optional group tcp {
		optional long mss,
		optional long flag
	},
	repeated group trans {
		optional string uri,
		optional int monitor_flag
	}
}
```



```json
Record 1: {
    "sid":"8509_1576752657",
    "appid":[81, 205, 67],
    "tcp": {
        "mss": 1750,
        "flag": 344
    },
    "trans":[{
            "uri":"/icon.jpg",
            "monitor_flag":1
        },{
            "uri":"/myyhp_2.2-4.js"
        }
    ]
}

Record 2: {
    "sid":"8510_1576752667",
    "appid":[58, 98]
}

Record 3: {
    "sid":"8511_1576754667",
    "appid":[198],
    "tcp": {
        "flag": 256
    }
}
```

D，即Definition Level，用于表达某个列是否为空、在哪里为空，其值为当前列在第几层上有值。对于required的字段，没有D值。

R，即Repetition Level，用于表达一个列有重复，即有多个值的情况，其值为重复是在第几层上发生。



## 压缩

```thrift
enum CompressionCodec {
  UNCOMPRESSED = 0;
  SNAPPY = 1;
  GZIP = 2;
  LZO = 3;
  BROTLI = 4; // Added in 2.4
  LZ4 = 5;    // Added in 2.4
  ZSTD = 6;   // Added in 2.4
}
```



