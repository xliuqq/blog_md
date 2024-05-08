[toc]

# Java Native

> Java Call Native：底层都是基于 JNI
>
> - 比较：https://github.com/jnr/jnr-ffi/blob/master/docs/ComparisonToSimilarProjects.md
>
> - [Java 21 FFI 性能测试 —— Panama vs JNI/JNA/JNR ](https://github.com/Glavo/java-ffi-benchmark)
>
> Native Call Java：底层都是基于 JNI.
>
> - JNI, JavaCPP.

## JNI(C)

标准的 Java native通信工具，但复杂度高，需要很多**native code到java的回调**。

详见 [JNI 使用文档](./jni.md)

- 性能最好，但是使用比较复杂；



## JNA(C)

> https://github.com/java-native-access/jna

- **基于[libffi](../../libffi.md)；基于反射，数据转换从本地代码跳到 Java 代码非常昂贵；**
- 不需要 JNI 代码 ，只需要定义接口和函数（匹配）；
- 相较于JNR，更为成熟；



## JNR(C)

> https://github.com/jnr/jnr-ffi

- **基于[libffi](../../libffi.md)**，但使用**字节码生成**技术而不是反射；
- 不需要JNI代码 ，只需要定义接口和函数（匹配）；

<img src="pics/jnr.png" alt="jnr" style="zoom:33%;" />



## Project Panama(C)

> JDK 19 ：[JEP 424: Foreign Function & Memory API (Preview) (openjdk.org)](https://openjdk.org/jeps/424)
>
> JDK 20 ：[JEP 434: Foreign Function & Memory API (Second Preview) (openjdk.org)](https://openjdk.org/jeps/434)



## JavaCPP(C++)

> JavaCPP provides efficient access to native <font color='red'>**C++**</font> inside Java， also access Java from C++.



