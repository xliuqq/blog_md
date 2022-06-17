# 使用



## Flink Core和App依赖

- Flink Core：`lib/flink-dist-*.jar`，不包含任何的connector的依赖；
- **User Application Dependencies** ：connectors，formats或者用户应用需要的jar包，不要包含flink的运行时jar包。





## 项目配置

flink的核心依赖，设置为provided。

```xml
<dependency>
  <groupId>org.apache.flink</groupId>
  <artifactId>flink-streaming-java_2.11</artifactId>
  <version>1.13.0</version>
  <scope>provided</scope>
</dependency>
```



## 添加依赖

使用*jar-with-dependencies*将依赖打包进去。

```xml
<dependency>
    <groupId>org.apache.flink</groupId>
    <artifactId>flink-connector-kafka_2.11</artifactId>
    <version>1.13.0</version>
</dependency>
```



## Scala版本

scala 2.11跟scala 2.12不兼容，scala 2.12.8之后版本跟之前的scala.2.12.x不兼容。



## Hadoop依赖

**General rule: It should never be necessary to add Hadoop dependencies directly to your application.**

Flink通过`HADOOP_CLASSPATH`指定Hadoop依赖

```bash
export HADOOP_CLASSPATH=`hadoop classpath`
```



## Hadoop Compatiable(TODO)

https://ci.apache.org/projects/flink/flink-docs-release-1.13/docs/dev/dataset/hadoop_compatibility/