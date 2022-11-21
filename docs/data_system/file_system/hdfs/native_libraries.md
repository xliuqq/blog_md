# Native Hadoop Library

**libhadoop.so**  要求：>zlib-1.2, >gzip-1.2

- Compression Codecs (bzip2, lz4, snappy, zlib)
- Native IO utilities for [HDFS Short-Circuit Local Reads](short_cut_read.md) and [Centralized Cache Management in HDFS](https://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-hdfs/CentralizedCacheManagement.html)
- CRC32 checksum implementation



## 下载

发行版的预编译的是**32位的i386 linux**本地hadoop库。



## 编译

ANSI C 源代码，使用GNU autotools工具链。

安装依赖：

- C compiler (e.g. GNU C Compiler)
- GNU Autools Chain: autoconf, automake, libtool
- zlib-development package (stable version >= 1.2.0)
- openssl-development package(e.g. libssl-dev)

编译

```shell
# 在根pom执行
mvn package -Pdist,native -DskipTests -Dtar
# 编译出来后在hadoop-dist/target/hadoop-3.2.1/lib/native
```



## 运行时

`bin/hadoop`脚本保证native hadoop库在库搜索路径上，通过**-Djava.library.path=<path>** 指定动态库位置

运行异常时日志：

INFO util.NativeCodeLoader - **Unable to load native-hadoop library** for your platform... using builtin-java classes where applicable



## 检查

NativeLibraryChecker是一个检查本地库是否正确加载的工具

```shell
 $ hadoop checknative -a
   14/12/06 01:30:45 WARN bzip2.Bzip2Factory: Failed to load/initialize native-bzip2 library system-native, will use pure-Java version
   14/12/06 01:30:45 INFO zlib.ZlibFactory: Successfully loaded & initialized native-zlib library
   Native library checking:
   hadoop: true /home/ozawa/hadoop/lib/native/libhadoop.so.1.0.0
   zlib:   true /lib/x86_64-linux-gnu/libz.so.1
   snappy: true /usr/lib/libsnappy.so.1
   zstd: true /usr/lib/libzstd.so.1
   lz4:    true revision:99
   bzip2:  false
```



## 本地共享库

使用 DistributedCache 分发和链接库文件：

1. 将库拷贝到HDFS； `bin/hadoop fs -copyFromLocal mylib.so.1 /libraries/mylib.so.1`

2. job启动程序应该包含：

   ```java
   DistributedCache.createSymlink(conf); 
   DistributedCache.addCacheFile("hdfs://host:port/libraries/mylib.so. 1#mylib.so", conf);
   ```

3. MapReduce任务可以使用：`System.loadLibrary("mylib.so");`



## [libHDFS BUG](https://issues.apache.org/jira/browse/HDFS-13585)

当在Java中通过JNI调用libhdfs访问hdfs时，出现 **Java -> C++ -> Java** 的调用栈，libhdfs在C++ pthread线程销毁时注册了JNIEnv的DetachCurrentThread，但是会在整个Java进程运行结束后JVM异常退出（hs_err文件），具体可见 [JNI的"Java调用C++再调用Java"](../../../languages/java/native/jni.md)。

- 但是**当调用 libhdfs的Java是new Thread时，不会造成JVM异常退出**；
- scala 要采用 `new Thread ()`的形式，而不能是`new Thread {}`的形式（JVM仍会Crash）；



## 使用

编译cpp文件命令

```shell
$ g++ -shared -fPIC -I${HADOOP_HOME}/include -L${HADOOP_HOME}/lib/native -lhdfs -L${JDK_HOME}/jre/lib/amd64/server -ljvm  -I ${JDK_HOME}/include/ -I ${JDK_HOME}/include/linux/ jni.cpp -o libjni.so
```

运行时需要如下的相关变量和参数：

```shell
export HADOOP_PREFIX=/home/experiment/workspace/hadoop/
export HADOOP_HOME=$HADOOP_PREFIX
export CLASSPATH=$($HADOOP_PREFIX/bin/hadoop classpath --glob)
export LD_LIBRARY_PATH=/home/experiment/workspace/hadoop/lib/native:/home/common/jdk/jre/lib/amd64/server
export LD_LIBRARY_PATH=/home/experiment/workspace/hadoop/lib/native:/home/common/jdk/jre/lib/amd64/server:.
```

