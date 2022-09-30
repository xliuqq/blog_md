[toc]

# 数据类型

## 基本类型

- BOOLEAN: 1位的boolean值
- INT32: 32位有符号整数
- INT64: 64位有符号整数
- INT96: 96位有符号整数（Deprecated）
- FLOAT: IEEE 32位浮点数
- DOUBLE: IEEE 64位浮点数
- BYTE_ARRAY: 任意长度的字节数组
- FIXED_LEN_BYTE_ARRAY: 固定长度的字节数组

## 逻辑类型

将实际语言使用的类型转换（逻辑类型）到Parquet支持的数据类型。

https://github.com/apache/parquet-format/blob/master/LogicalTypes.md

- **String **： binary field(String)；

- **ENUM** ：binary field(Enum)；MAP_KEY_VALUE：两个字段的group；

- **INTEGER** ：Int32 field(INTEGER(8,false))，表示存储的是无符号的int_8；

- **DECIMAL** ：Int64 decimal(precision, scale)，基本类型可以是Int32, Int64, binary, fixed_len_byte_array。Int32表示的精度为从1到9；

- **TIME, TIMESTAMP, INTERVAL**；

- **JSON** ：binary field(JSON)；

- **LIST** ：格式定义如下

  ```txt
  <list-repetition> group <name> (LIST) {
    repeated group list {
      <element-repetition> <element-type> element;
    }
  }
  ```

  - \<list-repetition> ：必须是optional或者required，表示该List是否可以为null；
  - \<element-repetition> ：必须是optional或者required，表示元素是否可以为null；

- **MAP**

  ```
  <map-repetition> group <name> (MAP) {
    repeated group key_value {
      required <key-type> key;
      <value-repetition> <value-type> value;
    }
  }
  ```

  - \<map-repetition> ：必须是optional或者required，表示该map是否可以为null；
  - \<value-repetition> ：必须是optional或者required，表示元素值是否可以为null；

## Nulls

null值在*definition level*定义（采用run-length编码）。数据中没有编码空值。例如，在非嵌套模式中，对于*definitition level*，具有1000个空值的列将使用运行长度编码(0,1000次)进行编码，而没有其他内容。

