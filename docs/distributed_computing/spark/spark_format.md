# Spark数据格式

## CSV

### Header

根据参数指定，`option("header", "true")`

### 数据类型

默认全部当作`String`处理；

参数支持自动推断，`option("inferSchema", "true")`

- 先将整个数据读取成DataSet[String]；
- 根据Header，将DataSet转成RDD[Array[String]]；
- 对RDD的每个元素中每个数据进行类型推断，并按规则聚合；
  - int -> long -> decimal -> double -> timestamp -> boolean -> string 的顺序；
  - 不同类型比较时，类型提升

