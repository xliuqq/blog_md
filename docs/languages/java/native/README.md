[toc]

# Java Native

> 比较：https://github.com/jnr/jnr-ffi/blob/master/docs/ComparisonToSimilarProjects.md

## JNI

标准的Java native通信工具，但复杂度高，需要很多**native code到java的回调**。

详见 [JNI 使用文档](./jni.md)

- 性能最好，但是使用比较复杂；
- Alluxio 使用 [JNI-Fuse](https://github.com/maobaolong/jnifuse) 替代 [JNR-Fuse](https://github.com/SerCeMan/jnr-fuse)；



## LibFFI

> https://github.com/libffi/libffi
>
> FFI（Foreign Function Interface）允许以一种语言编写的代码调用另一种语言的代码，而libffi库提供了最底层的、与架构相关的、完整的FFI。

`Libffi`是一个外部函数接口库。

- 提供了一个C编程语言接口，用于**在运行时（而不是编译时）给定有关目标函数的信息来调用本地编译函数**。
- 可以生成一个指向可以**接受和解码在运行时定义的参数组合的函数的指针**。

`libffi`的作用就相当于编译器，它为多种调用规则提供了一系列高级语言编程接口，然后通过相应接口完成函数调用，底层会根据对应的规则，完成数据准备，生成相应的汇编指令代码。



## JNA

> https://github.com/java-native-access/jna

- **基于libffi；基于反射，数据转换从本地代码跳到 Java 代码非常昂贵；**
- 相较于JNR，更为成熟；
- 不需要JNI代码 ，只需要定义接口和函数（匹配）；



## JNR

> https://github.com/jnr/jnr-ffi

- **基于libffi**，但使用**字节码生成**技术而不是反射；
- 不需要JNI代码 ，只需要定义接口和函数（匹配）；

<img src="pics/jnr.png" alt="jnr" style="zoom:33%;" />



