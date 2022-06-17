# Stream

JDK 8 引入，对于集合数据可以流式处理。

## 创建

Stream.of 可以创建Stream，集合类通过通过stream方法或者parallelStream创建流。



## 中间操作

peek

skip, sorted, map, flatMap, filter, distinct



## 末端操作

collect

count

max

min

reduce

forEach, forEachOrdered

findFirst, findAny

anyMatch, allMatch, noneMatch

toArray





## Collectors

groupingBy：按Key聚合

groupingByConcurrent

joining

partitioningBy

maxBy, minBy

toList, toMap, toSet, toConcurrentMap, toCollection



# Flow

JDK 9 中的反应流。

Flow.Publisher 接口：实现信息的发布者；

Flow.Subscriber接口：实现信息的订阅者；

Flow.Subsciption接口：发布和订阅的订阅关系；