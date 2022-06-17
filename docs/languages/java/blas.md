# Blas

**Java Blas库：**

- **[jblas]([http://jblas.org](http://jblas.org/))** is based on BLAS and LAPACK；
- [Apache Commons Math](http://commons.apache.org/proper/commons-math/) for the most popular mathematics library in Java (not using netlib-java）；
- **[Breeze](https://github.com/scalanlp/breeze)** for high performance linear algebra in Scala and Spark (builds on top of netlib-java 1.1.2)；

免费版不再提供：

- [Matrix Toolkits for Java](https://github.com/fommil/matrix-toolkits-java/) for high performance linear algebra in Java (builds on top of netlib-java)；

- [**netlib-java**](https://github.com/fommil/netlib-java) is a wrapper for low-level BLAS, LAPACK and ARPACK（不再开发免费版）；



**底层库安装**：

- yum install atlas

- yum install lapack



**scala使用demo** 

```scala
import breeze.linalg.DenseMatrix  
val m = DenseMatrix.rand(100, 100)  
val res = m * m  
```

scala -cp spark-assembly-1.4.0-hadoop2.6.0.jar

看到有类似一下信息输出则表明blas安装并调用成功：

Dec 03, 2015 12:59:42 AM com.github.fommil.jni.JniLoader liberalLoad

INFO: **successfully** loaded /tmp/jniloader4368196623437279381netlib-native_system-linux-i686.so