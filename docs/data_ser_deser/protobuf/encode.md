[toc]

# 编码

## 示例

简单的消息定义：

```protobuf
message Test1 {
  optional int32 a = 1;
}
```

在一个应用程序中，你创建一个 Test1 message，并将 a 设置为150。然后，将 message 序列化为输出流。如果你能够查看相应的编码后的结果，你会看到以下三个字节（16进制）：

```protobuf
08 96 01
```



## Base 128 Varints

varints 是一种用一个或多个字节序列化（编码）整数的方法。较小的数字将占用较少的字节数。

varint 中的每个字节都设置了一个标识位（msb）- msb 表示后面还有字节需要读取（当前编码还未结束）。每个字节的低 7 位用于以 7 位一组的方式存储数字的二进制补码，二进制补码组的低位排在编码序列的前面（**least significant group first**）。

```python
# 300 的编码
1010 1100 0000 0010   # 去除每个字节的msb
→ 010 1100  000 0010  # 将两个7位组反转，前面的组表示低位
→ 000 0010  010 1100  # 去掉计算时多余的 0     
→ 100101100           # 计算对应的整型       
→ 256 + 32 + 8 + 4 = 300    
```

> varints 之所以将低位的放在前头，是为了进行位操作（还原数值）的时候更加方便。



## Message结构

一个 protocol buffer message 实际上是一系列的键值对。消息的二进制版本只**使用字段的数字作为 key** - 而每个**字段的名称和声明的类型只能通过引用 message 类型的定义（即 .proto 文件）在解码端确定**。

识别 message 编码中每个字段的 key 实际上是两个值 - 来自 .proto 文件的**字段编号**，以及一个提供足够信息以查找 *“值的（字节）长度”* 的**类型**。在大多数语言实现中，该 key 被称为一个 **tag （标记）**。

可用的**类型**如下:

| Type |     Meaning      |                         Used For                         |
| :--: | :--------------: | :------------------------------------------------------: |
|  0   |      Varint      | int32, int64, uint32, uint64, sint32, sint64, bool, enum |
|  1   |      64-bit      |                fixed64, sfixed64, double                 |
|  2   | Length-delimited | string, bytes, embedded messages, packed repeated fields |
|  3   |   Start group    |                 groups (deprecated,遗弃)                 |
|  4   |    End group     |                 groups (deprecated,遗弃)                 |
|  5   |      32-bit      |                 fixed32, sfixed32, float                 |

消息流中的每个 **Tag (field_number + wire_type)** 都使用 varint 进行编码，且最后三位 bit 存储**类型 wire_type**（其它位存储**字段编号 field_number**）。

示例中，第一个数字总是一个 varint 编码的 tag ，这里是 08 (000 1000)，后三位 bit 从而得到类型为 0 (varint，接下来要读取的 value 的类型)，右移 3 位得到 varint 编码的 1（字段编号）

```rust
96 01 = 1001 0110  0000 0001
       → 000 0001  +  001 0110 (drop the msb and reverse the groups of 7 bits)
       → 10010110
       → 128 + 16 + 4 + 2 = 150
```

## Signed Integers

在对负数进行编码时，signed int 类型（sint32 和 sint64）与标准的 int 类型（int32 和 int64）之间存在着重要差异。如果**使用 int32 或 int64 作为负数的类型，则生成的 varint 总是十个字节长**－它实际上被视为一个非常大的无符号整数。如果**使用 signed int 类型（sint32 和 sint64），则生成的 varint 将使用 ZigZag 编码**，这样效率更高。

> 如果是负数，那么必须有最高位表示符号位，也就是说天然的会用尽所有字节(每个字节还需要拿出一个 bit 位做 msb，最多5个字节）。
>
> protobuf 为了兼容性， int32 和 int64 能够兼容（比如在 .proto 文件中将 int32 改成 int64），所以 int32 的负数会扩展到 int64 的长度（10字节）。
>
> 正数的兼容：varints 编码，这种编码天然使得 int32 和 int64 的正数兼容。

ZigZag的说明见[ZigZag编码.md](../../data_format/encoding.md#zigzag)，ZigZag 编码将有符号整数映射到无符号整数。

| Signed Original | Encoded As |
| :-------------- | :--------- |
| 0               | 0          |
| -1              | 1          |
| 1               | 2          |
| -2              | 3          |
| 2147483647      | 4294967294 |
| -2147483648     | 4294967295 |

 sint32 类型的 n 编码处理如下：

```undefined
(n << 1) ^ (n >> 31)
```

## Non-varint Numbers

non-varint 数字类型很简单，double 和 fixed64 对应的类型（wire type）为 1（读取64为位），float 和 fixed32 对应的类型（wire type）为 5（读取32位）。

值都以 little-endian （二进制补码低位在前）字节顺序存储。

## Strings

类型（wire type）为 2（长度划分）表示**接下来的字节将是 varint 编码的长度，之后跟指定长度的数据字节**。

```protobuf
message Test2 {
  optional string b = 2;
}
```

将 b 的值设置为 "testing" 后得到如下编码：

```protobuf
12 07 | 74 65 73 74 69 6e 67
```

后七个字节为 "testing" 的 UTF8 编码。第一个字节 0x12 为 key → 字段编码 field_number = 2, 类型 wire type = 2。 varint 值的长度为 7，并且看到它后面有七个字节 -即为我们需要读取的值（字符串）。

## Embedded Messages

下面的 message，内嵌了我们之前的简单例子 Test1：

```protobuf
message Test3 {
  optional Test1 c = 3;
}
```

设置其中的 Test1 的 a = 150，最终编码如下：

```protobuf
1a 03 08 96 01
```

后三个字节和我们的第一个例子结果相同（08 96 01），在这三个字节之前为 03 编码（代表着字节长度）-嵌入消息的处理方式与字符串完全相同（1a表示序号位3和wire type为2）。

## Optional 和 Repeated 元素

proto2 message 定义有 `repeated` 字段（没有使用 `[packed=true]` 选项），则对应的 message 编码将具有**零个或多个相同字段编号的键值对**。

对于 proto3 中的任何 **non-repeated 字段**或 proto2 中的 optional 字段，message 编码**可能具有或不具有该字段编号对应的键值对**。

通常，编码消息永远不会有多个 non-repeated 字段的实例，但解析器应该能够处理这个情况。对于数字类型和字符串，如果多次出现相同的字段，则解析器接受它 “看到” 的最后一个值。

对于**嵌入式消息字段，解析器合并同一字段的多个实例**，就像使用 Message::MergeFrom 方法一样 - 也就是说，后面实例中的所有**单个标量字段都替换前者，单个嵌入消息被合并，而 repeated 字段将被串联起来**。这些规则的作用是使得两个消息（字符串）串联起来解析产生的结果与分别解析两个消息（字符串）后合并结果对象产生的结果完全相同。也就是：

```css
MyMessage message;
message.ParseFromString(str1 + str2);
```

等价于：

```css
MyMessage message, message2;
message.ParseFromString(str1);
message2.ParseFromString(str2);
message.MergeFrom(message2);
```

此属性偶尔会有用，因为它允许我们合并两条消息，即使我们不知道它们的类型。

### Packed Repeated Fields

在 proto3 中，`repeated` 字段默认使用 packed 编码。

**没有包含任何元素的 packed repeated 字段不会出现在编码消息中**。否则（包含有元素），该字段的**所有元素都被打包成一个键值对，其中类型（wire type）为 2（length-delimited）**。每个**元素的编码方式与正常情况相同，只是前面没有键**。

例如，假设我们有如下消息类型：

```protobuf
message Test4 {
  repeated int32 d = 4 [packed=true];
}
```

现在假设你构造一个 Test4，为重复的字段 d 分别赋值为 3、270 和 86942。那么对应的编码将是：

```cpp
22        // key (或称 tag，字段编码 4, wire type 2)
06        // payload 长度 (即后面需要读取的所有值的长度6 bytes)
03        // first element (varint 3)
8E 02     // second element (varint 270)
9E A7 05  // third element (varint 86942)
```

**只有原始数字类型（使用 Varint，32-bit 或 64-bit的wire type）的 repeated 字段才能声明为 “packed”。**

>repeated string 类型不会被打包？
>
>对应的 packed 结果如果是 tag-length-value-value-value...，则无法识别数组元素的边界。
>
>如果 packed 结果为 tag-length-[length-value-length-value]，那么和不打包的编码结果 tag-length-value-tag-length-value-tag-length-value.... 相比 ？？

