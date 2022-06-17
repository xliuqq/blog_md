# 向量化介绍

## Java向量化

向量化代码：

```java
for (int i-0; i < pos.length; i++) {
    pos[i] = pos[i] + mov[i];
}
```

Java代码向量化执行：

- **自动的**：**不能手动控制，只能由 JVM 自动处理。**没有办法做到像 C++ 一样直接调一个底层的 CPU 指令；
- **隐式的**：**代码层面无法找到向量化的显式调用，整个过程是隐式的**，比如上面的 for 循环被向量化执行；
- **不可靠的**：**依赖于 JVM 运行期的热点代码跟踪以及 JIT ，所以整个过程是不可靠的**；



## 火山迭代模型（Volcano Iterator Model）

现在大多数大数据系统或者说数据库底层，对 SQL 进行处理时通常会采用的模型。

模型具有**易抽象、易实现**、以及能够通过**算子组合表达复杂查询**这三个优势。

- 大量虚函数的调用，就可能会导致 CPU 的中断和耗时

<img src="pics/image-20220429162701122.png" alt="image-20220429162701122" style="zoom:67%;" />



## Spark Code Generation

- 减少基本类型的自动装箱；
- 避免多态调用（火山迭代模型）；
- 利用SIMD批量处理数据（JVM向量化）；
- 其它Fix（算子融合/缩减栈深）；



### 整体Stage代码生成（Whole-stage code generation）

**使得计算引擎的物理执行速度能达到 hard code 的性能**：

- 对物理执行的多次调用转换为代码 for 循环，减少中间执行的函数调用次数

示例：*select count(\*) from store_sales where ss_item_sk = 1000*

通常物理计划的代码是这样实现的：

```scala
class Filter(child: Operator, predicate: (Row => Boolean)) {
	def next(): Row = {
        var current = child.next()

        while (current != null && !predicate(current)) {
            current == child.next();
        }
        return current
    }
}
```

但是真正如果我们用 hard code 写的话，代码是这样的：

```scala
var count = 0
for (ss_item_sk in store_sales) {
    if (ss_item_sk == 1000) {
        count += 1
    }
}
```

原因：

- 避免**virtual function dispatch**；next()等函数调用在操作系统层面，会被编译为virtual function dispatch。
- 通过**CPU Register存取中间数据**，而不是内存缓冲：在Volcano Iterator Model中，每次一个operator将数据交给下一个operator，都需要将数据写入内存缓冲中。然而在手写代码中，JVM JIT编译器会将这些数据写入CPU Register。
- **Loop Unrolling**和**SIMD**；