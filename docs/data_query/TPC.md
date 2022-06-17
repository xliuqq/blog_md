# TPC 

## TPC-DS

[TPC-DS](http://www.tpc.org/tpc_documents_current_versions/current_specifications.asp)是一个面向决策支持系统(decision support system)的包含多维度常规应用模型的决策支持基准，包括查询(queries)与数据维护。

采用星型、雪花型等多维数据模式。它包含7张事实表，17张纬度表平均每张表含有18列。

其工作负载包含99个SQL查询，覆盖SQL99和2003的核心部分以及OLAP。

这个测试集包含对大数据集的统计、报表生成、联机查询、数据挖掘等复杂应用，**测试用的数据和值是有倾斜的，与真实数据一致**。可以说TPC-DS是与真实场景非常接近的一个测试集，也是难度较大的一个测试集。

TPC-DS的这个特点跟大数据的分析挖掘应用非常类似。Hadoop等大数据分析技术也是对海量数据进行大规模的数据分析和深度挖掘，也包含交互式联机查询和统计报表类应用，同时大数据的数据质量也较低，数据分布是真实而不均匀的。因此TPC-DS成为客观衡量多个不同Hadoop版本以及SQL on Hadoop技术的最佳测试集。这个基准测试有以下几个主要特点：

- 一共99个测试案例，遵循SQL’99和SQL 2003的语法标准，SQL案例比较复杂；
- 分析的数据量大，并且测试案例是在回答真实的商业问题；
- 测试案例中包含各种业务模型（如分析报告型，迭代式的联机分析型，数据挖掘型等）；
- 几乎所有的测试案例都有很高的IO负载和CPU计算需求；

### 安装使用

- 下载（https://github.com/gregrahn/tpcds-kit，官网的可能有些bug）；
- 解压并进入`tools`，执行`make`命令；
  - 生成`dsdgen, dsqgen, distcompare, checksum, mkheader`等命令；

### 数据生成

```shell
dsdgen -DIR /path/store -SCALE 1
```
参数：

- -DIR：生成文件的目录；
- -SCALE：数据规模，以GB为基本单位1；
- -PARALLEL：生成的数据一共分为多少份，一般生成TB级数据才会用到。
- -CHILD：当前数据是第几份，与PARALLEL配对使用。
- -DELIMITER：指定分隔符，默认'|'；
- -TERMINATE：是否每个记录末尾以分隔符结束；

会在目录中生成25个.dat文件。

### 数据导入



### SQL生成

```shell
for i in `seq 1 99`; do 
	./dsqgen  -DIRECTORY ../query_templates/ -TEMPLATE "query${i}.tpl" -DIALECT netezza -FILTER Y > ../sql/query${i}.sql; 
done
```

参数说明：

- -DIRECTORY：SQL模板的路径；
- -TEMPLATE：SQL模板的名称；
- -DIALECT：不同的SQL语法，用netezza；
- -FILTER：将生成的SQL写到控制台；



## FAQ

**为什么不直接用TPC官网下载**？

官网直接下载后生成SQL语句会出现：

> ERROR: Substitution'_END' is used before being initialized at line ...

解决方式：(批量在每个query语句后面添加`define _END = "";`或者在对应的DIALECT中添加)

```bash
#!/bin/bash
COUNTER=1
while [ $COUNTER -lt 100 ]
do
echo $COUNTER
echo "define _END = \"\";">>query$COUNTER.tpl
COUNTER=`expr $COUNTER + 1`
done    
```

