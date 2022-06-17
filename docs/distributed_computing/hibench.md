# HiBench

https://github.com/Intel-bigdata/HiBench



## 数据集生成

1. 基础配置

   ```shell
   ${HiBench}/conf/hadoop.conf  #指定Hadoop安装位置等信息
   ${HiBench}/conf/spark.conf #指定Spark安装位置等信息
   ```

2. 生成数据集作业配置

   **生成输入数据集大小等相关配置**

   `${HiBench}/conf/workloads/micro/wordcount.conf`

   ```shell
   # 路径根据数据规模输出文件
   hibench.workload.input  ${hibench.hdfs.data.dir}/wordcount/${hibench.scale.profile}
   ```

   **数据生成脚本相关配置**

   ${HiBench}/conf/hibench.conf`

   ```shell
   # 主要修改文件开头这三项
   hibench.scale.profile               bigdata
   # Mapper number in hadoop, partition number in Spark
   hibench.default.map.parallelism        8
   # Reducer nubmer in hadoop, shuffle partition number in Spark（实验环境是8个）
   hibench.default.shuffle.parallelism     8
   
   # 输出的数据根目录，不用修改，默认即可
   hibench.hdfs.data.dir           ${hibench.hdfs.master}/HiBench
   ```

3. 数据生成脚本

   ```shell
   # 配置完成后运行脚本即可生成作业，注意yarn-site.xml配置中'yarn.resource.'的配置中不要添加注释
   ${HiBench}/bin/workloads/micro/wordcount/prepare/prepare.sh
   # 修改hibench.conf中hibench.scale.profile的值(tiny, small, large, huge, gigantic),每次修改后,重新执行prepare.sh
   ```

4. 查看数据集结果

   ```shell
   hadoop fs -ls /HiBench/wordcount/
   # 该目录下应该有tiny, small, large, huge, gigantic
   ```

   