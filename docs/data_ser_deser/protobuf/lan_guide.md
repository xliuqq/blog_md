[toc]

# 语法指引(proto3)

> protobuf 不支持类继承。

## 消息类型定义

消息定义中每个字段都有**唯一的编号**，用于标识**消息二进制**的字段，字段号不应更改。

- 范围1-15 编号需要一个字节来编码，包括字段编号和字段的类型；

- 16到2047之间的字段编号占用两个字节；

- **为经常出现的消息元素保留数字1到15。切记为将来可能添加的频繁出现的元素留出一些空间**。

```protobuf
syntax = "proto3";
message xxx {
  // 字段规则：默认无修饰符 -> 字段可出现 0 次或1次
  // 字段规则：repeated -> 字段可出现任意多次（包括 0）
  // 类型：int32、int64、sint32、sint64、string、bytes ....
  // 字段编号：1 ~ 536870911（2^29 - 1, 除去 19000 到 19999 之间的数字）
  字段规则 类型 名称 = 字段编号;
}
```

在proto3中，`repeated`修饰的数值变量默认采用`packed`编码。

单个`.proto`文件中，可以定义多个message，message中也可以嵌套定义message，嵌套类型在其它message中使用为 *`Parent.Type`*。

## 字段修饰

- `singular`：默认修饰，0或者1个；
- `repeated`：任意多个（包含0）；

**保留字段**

如果通过删除/注释字段来更新消息，则将来用户可能会重用这些该字段编号。若以后他们加载了旧版本`.proto`文件，则会出现数据损坏、隐私错误等严重问题。 解决方法是，**将已删除字段的字段编号（和/或名称，也可能导致JSON序列化的问题）指定为`reserved`** ，如果后续重新使用该字段，则编译器会警告。

```protobuf
message Foo {
  reserved 2, 15, 9 to 11;
  reserved "foo", "bar";   // 编号和名字要分开写
}
```

## 枚举

枚举值使用variant encoding。

```protobuf
message SearchRequest {
  string query = 1;
  int32 page_number = 2;
  int32 result_per_page = 3;
  enum Corpus { 
    option allow_alias = true; // 定义alias，枚举值可以相同
    UNIVERSAL = 0;   // 枚举值必须从0开始
    WEB = 1;
    IMAGES = 2;
    LOCAL = 2;       // alias，别名
  }
  Corpus corpus = 4;
}
```

## 使用其它的消息类型

如果使用到其它.proto中定义的message类型，使用 import 语句：

```protobuf
import "myproject/other_protos.proto";
```

protocol编译器使用 `-I`/`--proto_path` 搜索`.proto`文件路径。

如果`.proto`需要被移动到其它位置，可以通过`import public`，而不需要在所有使用它的`.proto`文件中，修改`import`的路径。

```protobuf
// new.proto
// All definitions are moved here
```

```
// old.proto
// This is the proto that all clients are importing.
import public "new.proto";
import "other.proto";
```

```protobuf
// client.proto
import "old.proto";
// You use definitions from old.proto and new.proto, but not other.proto
```

## 更新消息类型

更新的规则：

- 不要更改现有字段的编号；
- 添加新字段时，记住默认值，以便新代码和旧代码生成的类型可以交互，而旧代码会忽略新的类型；
- 确定编号不再使用的类型可以删除，或者使用` OBSOLETE_ `修饰，或者使用`reserverd`。

## Any类型(development)

表示任意类型，使用bytes存储，并且有URL作为全局唯一标识。**等价于proto2中的extensions**。

```protobuf
import "google/protobuf/any.proto";

message ErrorStatus {
  string message = 1;
  repeated google.protobuf.Any details = 2;
}
```

 <font color="red">**Currently the runtime libraries for working with Any types are under development(2019/11/1)**. </font>

## Oneof

类似于C中union类型。oneof 里面不能使用`repeated`字段。

```protobuf
message SampleMessage {  
	oneof test_oneof {
    	string name = 4;
        SubMessage sub_message = 9;
    }
}
```

**TODO：根据实际使用，分析兼容性问题**

## Map

key_type类型是整型或string类型（浮点数、bytes、enum不行），value_type可以是除map的任意类型。

```protobuf
map<key_type, value_type> map_field = N;
```

- Map的字段不能为`repeated`；

- Map的遍历是无序的；

## Package

```protobuf
package foo.bar;
message Open { ... }
```

```protobuf
message Foo {
  ...
  foo.bar.Open open = 1;
  ...
}
```

## Defining Services

如果你在 .proto 文件中定义 RPC 服务，你应该使用驼峰命名法（首字母大写）命名 RPC 服务以及其中的 RPC 方法： 

```protobuf
service SearchService {
  rpc Search (SearchRequest) returns (SearchResponse);
}
```

配合grpc或者[第三方实现](https://github.com/protocolbuffers/protobuf/blob/master/docs/third_party.md)。

## Options



-  `java_package`  (file option) ：生成的java类的包名；

-  `java_multiple_files`  
-  `java_outer_classname`  
-  `optimize_for`  
-  `cc_enable_arenas`  
-  `objc_class_prefix`  
-  `deprecated`  

**自定义option**

使用proto2的[extensions](https://developers.google.cn/protocol-buffers/docs/proto.html#extensions)。

## 生成语言的类文件

使用protoc命令

```shell
protoc --proto_path=IMPORT_PATH --java_out=DST_DIR path/to/file.proto
```

## JSON Mappings



| proto3                 | JSON          | JSON example                              | Notes                                                        |
| ---------------------- | ------------- | ----------------------------------------- | ------------------------------------------------------------ |
| message                | object        | `{"fooBar": v,"g": null,…}`               | Generates JSON objects. Message field names are mapped to lowerCamelCase and become JSON object keys. If the `json_name` field option is specified, the specified value will be used as the key instead. Parsers accept both the lowerCamelCase name (or the one specified by the `json_name` option) and the original proto field name. `null` is an accepted value for all field types and treated as the default value of the corresponding field type. |
| enum                   | string        | `"FOO_BAR"`                               | The name of the enum value as specified in proto is used. Parsers accept both enum names and integer values. |
| map<K,V>               | object        | `{"k": v, …}`                             | All keys are converted to strings.                           |
| repeated V             | array         | `[v, …]`                                  | `null` is accepted as the empty list [].                     |
| bool                   | true, false   | `true, false`                             |                                                              |
| string                 | string        | `"Hello World!"`                          |                                                              |
| bytes                  | base64 string | `"YWJjMTIzIT8kKiYoKSctPUB+"`              | JSON value will be the data encoded as a string using standard base64 encoding with paddings. Either standard or URL-safe base64 encoding with/without paddings are accepted. |
| int32, fixed32, uint32 | number        | `1, -10, 0`                               | JSON value will be a decimal number. Either numbers or strings are accepted. |
| int64, fixed64, uint64 | string        | `"1", "-10"`                              | JSON value will be a decimal string. Either numbers or strings are accepted. |
| float, double          | number        | `1.1, -10.0, 0, "NaN", "Infinity"`        | JSON value will be a number or one of the special string values "NaN", "Infinity", and "-Infinity". Either numbers or strings are accepted. Exponent notation is also accepted. |
| Any                    | `object`      | `{"@type": "url", "f": v, … }`            | If the Any contains a value that has a special JSON mapping, it will be converted as follows: `{"@type": xxx, "value": yyy}`. Otherwise, the value will be converted into a JSON object, and the `"@type"` field will be inserted to indicate the actual data type. |
| Timestamp              | string        | `"1972-01-01T10:00:20.021Z"`              | Uses RFC 3339, where generated output will always be Z-normalized and uses 0, 3, 6 or 9 fractional digits. Offsets other than "Z" are also accepted. |
| Duration               | string        | `"1.000340012s", "1s"`                    | Generated output always contains 0, 3, 6, or 9 fractional digits, depending on required precision, followed by the suffix "s". Accepted are any fractional digits (also none) as long as they fit into nano-seconds precision and the suffix "s" is required. |
| Struct                 | `object`      | `{ … }`                                   | Any JSON object. See `struct.proto`.                         |
| Wrapper types          | various types | `2, "2", "foo", true, "true", null, 0, …` | Wrappers use the same representation in JSON as the wrapped primitive type, except that `null` is allowed and preserved during data conversion and transfer. |
| FieldMask              | string        | `"f.fooBar,h"`                            | See `field_mask.proto`.                                      |
| ListValue              | array         | `[foo, bar, …]`                           |                                                              |
| Value                  | value         |                                           | Any JSON value                                               |
| NullValue              | null          |                                           | JSON null                                                    |
| Empty                  | object        | {}                                        | An empty JSON object                                         |

## **标量类型映射到语言类型**

**下面只列出，常用的语言的对应类型，更多类型请访问[官网](https://developers.google.cn/protocol-buffers/docs/proto3?hl=en#scalar)**

| **.proto Type** | **Notes**                                              | **C++ Type** | **Java Type** | **Python Type** | **Go Type** |
| ----------- | ------------------------------------------------------ | -------- | ---------- | -------------- | ------- |
| **double**  |                                                        | **double** | **double** | **float**      | **float64** |
| **float**   |                                                        | **float** | **float**  | **float**      | **float32** |
| **int32**   | **变长编码，编码负数效率不高。负数情况使用sint32。**   | **int32** | **int**    | **int**        | **int32** |
| **int64**   | **变长编码，编码负数效率不高。负数情况使用sint32。**   | **int64** | **long**   | **int/long** | **int64** |
| **uint32**  | **变长编码**                                           | **uint32** | **int**    | **int/long** | **uint32** |
| **uint64**  | **变长编码**                                           | **uint64** | **long**   | **int/long** | **uint64** |
| **sint32**  | **变长编码，有符号Int。编码负数比int32效率高。**       | **int32** | **int**    | **int**        | **int32** |
| **sint64**  | **变长编码，有符号Int。编码负数比int64效率高。**       | **int64** | **long**   | **int/long** | **int64** |
| **fixed32** | **固定四字节。值大于2<sup>28</sup>，比unit32效率高。** | **uint32** | **int** | **int/long** | **uint32** |
| **fixed64** | **固定八字节。值大于2<sup>56</sup>，比unit64效率高。** | **uint64** | **long** | **int/long** | **uint64** |
| **sfixed32** | **固定四字节**                                         | **int32** | **int**    | **int**        | **int32** |
| **sfixed64** | **固定八字节**                                         | **int64** | **long**   | **int/long** | **int64** |
| **bool**    |                                                        | **bool** | **boolean** | **bool**       | **bool** |
| **string**  | **UTF-8编码或7-bit ASCII的文本。不能大于2<sup>32</sup>。** | **string** | **String** | **str/unicode** | **string** |
| **bytes**   | **任意字节的byte，长度小于2<sup>32</sup>.**            | **string** | **ByteString** | **str**        | **[]byte** |