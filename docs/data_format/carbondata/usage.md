[toc]

# 使用

## Java SDK

http://carbondata.apache.org/sdk-guide.html

需要carbondata-store-sdk, carbondata-core, carbondata-format, carbondata-common, carbondata-hadoop，但maven只需引入

```xml
<dependency>
    <groupId>org.apache.carbondata</groupId>
    <artifactId>carbondata-store-sdk</artifactId>
    <version>1.6.1</version>
</dependency>
<dependency>
    <groupId>org.apache.carbondata</groupId>
    <artifactId>carbondata-core</artifactId>
    <version>1.6.1</version>
</dependency>
```

**CarbondataTest**类 Demo

- 支持CSV, AVRO, JSON三种格式的存储。

- 读取，支持ArrowCarbonReader和CarbonReader；

读取的时候可以进行列选择和值过滤（谓词下推）

```java
CarbonReader reader = CarbonReader
                .builder(path, "_temp")
                .projection(new String[]{"name", "age"})
                .filter(new GreaterThanExpression(new ColumnExpression("age", DataTypes.INT),
                        new LiteralExpression(2, DataTypes.INT)))
                .build();
```



## Spark集成

**carbon**可以作为Spark的一种数据源使用（类似Parquet）。carbondata格式是carbon自己使用的格式。

### Standalone

- carbondata项目源码编译生成 `./assembly/target/scala-2.1x/carbondata_xxx.jar`；

- 将carbondata的jar包添加到spark的所有节点的classpath中；

- 配置`carbon.properties`，

  - **carbon.storelocation**：存储数据的目录，默认**spark.sql.warehouse.dir**；

- `carbon.properties`传到各个节点，在executor和driver启动时指定

  `-Dcarbon.properties.filepath = ${PATH_TO}/carbon.properties`

### Yarn

- carbondata的编译后的jar包，并创建目录carbonlib，移入后再进行压缩；

  ```shell
  cd $SPARK_HOME
  mv carbon_*.jar carbonlib/
  tar -zcvf carbondata.tar.gz carbonlib/
  mv carbondata.tar.gz carbonlib/
  ```

- 配置carbon.properties，**carbon.storelocation**;

- 配置Yarn
  - **spark.yarn.dist.files**：指向 carbon.properties 的路径；

  - **spark.yarn.dist.archives**：`$SPARK_HOME/carbonlib/carbondata.tar.gz`

  - **spark.executor.extraClassPath**：`carbondata.tar.gz/carbonlib/*`

  - **spark.driver.extraClassPath**：`$SPARK_HOME/carbonlib/*`

  - 配置executor和driver的JVM参数

    `-Dcarbon.properties.filepath = carbon.properties`



## Thrift查询

```shell
./bin/spark-submit \
--class org.apache.carbondata.spark.thriftserver.CarbonThriftServer \
$SPARK_HOME/carbonlib/$CARBON_ASSEMBLY_JAR <carbon_store_path>
```

默认的是多session模式。



## Presto

**注意：Presto不能创建或加载CarbonData表；**只能通过Spark或者Java/C++ SDK创建。



## IDEA下开发

Carbondata是作为spark的一种数据源使用，因此和spark版本紧耦合。

因此其maven依赖很难指定（因为其版本不清楚和哪个spark版本兼容）。

建议：项目的开发从官网下载对应spark版本carbon的fat jar，然后导入作为IDEA的依赖。



1.6.1下的坑

- **carbon 1.6.1虽然默认的profile是2.2.1，但是仓库中下载的确实和spark2.3.2匹配的jar包；**