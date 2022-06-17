# HDFS

## 配置

```shell
# 设置libjvm.so 和 libhdfs.so的路径
LD_LIBRARY_PATH=$HADOOP_HOME/lib/native:$JAVA_HOME/jre/lib/amd64/server/:$LD_LIBRARY_PATH
# 设置 hdfs 的jar包路径
CLASSPATH=`hdfs classpath --glob`
```

配置后，TF的模型和检查点就可以读写HDFS。