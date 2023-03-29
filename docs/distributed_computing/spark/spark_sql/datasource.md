# 数据源



## 自定义数据源

> [Spark原理图解：DataSource V1 API与自定义数据源](https://mp.weixin.qq.com/s/gSRchNJNPo6STVw4rDyZeg)

### DataSource V1与V2

Spark 2.3以前数据源相关的API一般叫做V1 API，它以RDD为基础，向上扩展schema信息，形成DataFrame。

之后的版本则是另一种实现思路，直接基于新的接口在DataFrame的思路上提供数据。



在`DataFrameReader`中，会先找 V2 实现，找不到则会再找 V1 实现；

```scala
DataSource.lookupDataSourceV2(source, sparkSession.sessionState.conf)
		  .map {...}
          .getOrElse(loadV1Source(paths: _*))
```



### V1自定义

关键点:

- `DataSourceRegister`，标识是数据源服务类，Spark会以它来扫描实现类；
- RelationProvider，标识是关系型的数据源，可以在Spark SQL中使用；
- BaseRelation，描述DataFrame的Schema信息 ；
- TableScan, 提供无参的数据扫描服务；PrunedScan，提供列裁剪的数据扫描服务；PrunedFilteredScan，提供列裁剪和过滤下推的数据扫描服务。三个scan接口任选其一

流程：

- `loadV1Source`中调用`DataSource`的`apply`进行初始化，并调用其`resolveRelation`创建`BaseRelation`，然后通过`SparkSession`创建`DataFrame`返回；

自定义

- 实现类必须继承DataSourceRegister，否则无法使用简称自动扫描到该实现类
- 实现类需要继承RelationProvider，才能标识为关系型的数据源，在SparkSQL才能使用
- shortName中的简称用于使用时format中指定格式
- 一般在实现中会通过option()传递自定义参数，参数会传入parameters
- 在createRelation需要获取相关信息，创建Schema，这里写死模拟了一下。正常如果是mysql之类的，需要连接数据库获取对应表的字段以及类型信息。

```scala
class TestProvider extends DataSourceRegister with RelationProvider {
    override def shortName(): String = "test"

    override def createRelation(sqlContext: SQLContext,
                                parameters: Map[String, String]): BaseRelation = {
        // 1 通过参数等生成schema信息
        val schema = StructType(Seq(
            StructField("name", StringType, nullable = true),
            StructField("age", IntegerType, nullable = true),
            StructField("address", StringType, nullable = true)
        ))

        // 2 创建BaseRelation
        TestRelation(schema)(sqlContext.sparkSession)
    }
}

case class TestRelation(override val schema: StructType)
                       (@transient val spark: SparkSession)
    extends BaseRelation with PrunedFilteredScan {
    override def sqlContext: SQLContext = spark.sqlContext

    override def buildScan(requiredColumns: Array[String],
                           filters: Array[Filter]): RDD[Row] = {
        // 构建查询返回RDD，根据requiredColumns进行列裁剪，filters进行谓词下推
        spark.createDataFrame(Seq(Stud("a", 10, "aaa"), Stud("b", 20, "bbb")))
            .rdd
    }
}

case class Stud(name: String, age: Int, address: String)

```

使用，注意还需要单独配置SPI的发现文件：

```scala
import org.apache.spark.sql.SparkSession

object Test2 {
  def main(args: Array[String]): Unit = {
    val spark: SparkSession = SparkSession.builder()
      .master("local")
      .getOrCreate()

    val df = spark.read.format("xingoo")
      .load()

      df.select("name")
        .filter("age > 15")
        .show(false)
  }
}
```



## V2 自定义（TODO）

