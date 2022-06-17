# Pyarrow

## HDFS

环境配置

- `HADOOP_HOME`: the root of your installed Hadoop distribution. Often has **lib/native/libhdfs.so**.

- `JAVA_HOME`: the location of your Java SDK installation.

- `ARROW_LIBHDFS_DIR` (optional): explicit location of `libhdfs.so` if it is installed somewhere other than `$HADOOP_HOME/lib/native`.

- `CLASSPATH`: must contain the Hadoop jars. You can set these using:

  ```
  export CLASSPATH=`$HADOOP_HOME/bin/hdfs classpath --glob`
  ```

  If `CLASSPATH` is not set, then it will be set automatically if the `hadoop` executable is in your system path, or if `HADOOP_HOME` is set.

  > 如果设置`HADOOP_HOME`，则 linux上会执行 `find -L $HADOOP_HOME -name *.jar`，find的结果的顺序是不确定的；
  >
  > 如果没设置`HADOOP_HOME`，则通过`hadoop classpath --glob`搜索；

