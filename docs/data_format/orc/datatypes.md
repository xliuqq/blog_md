[toc]

# 数据类型

- Integer
  - boolean (1 bit)
  - tinyint (8 bit)
  - smallint (16 bit)
  - int (32 bit)
  - bigint (64 bit)
- Floating point
  - float
  - double
- String types
  - string
  - char
  - varchar
- Binary blobs
  - binary
- Date/time
  - timestamp
  - timestamp with local time zone
  - date
- Compound types
  - struct
  - list
  - map
  - union

复合类型的子列包含子元素的值。例如，对于结构的每个字段，结构列都有一个子列。列表(List)总是有一个子列用于元素值，而映射(Map)总是有两个子列。联合(Union)列对于每个变体都有一个子列。



## 示例

```sql
create table Foobar (
 myInt int,
 myMap map<string,
 struct<myString : string,
 myDouble: double>>,
 myTime timestamp
);
```

<img src="pics/TreeWriters.png" alt="ORC column structure" style="zoom:67%;" />