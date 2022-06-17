# Doctor Elephant

**Dr. Elephant** is a **performance monitoring** and **tuning** tool for Hadoop and Spark.

方式：

- 收集 job 的metrics；
- rule-based 启发式；
- Yarn Resource Manager， Job History Server；
- 



Azkaban调度 ？？支持Hadoop任何调度，如Oozie







## 使用

前提：

activator command，MYSQL，npm

python：inspyred



$> export HADOOP_HOME=/path/to/hadoop/home 

$> export HADOOP_CONF_DIR=$HADOOP_HOME/etc/hadoop 

$> export SPARK_HOME=/path/to/spark/home 

$> export SPARK_CONF_DIR=/path/to/conf

$> export PATH=$HADOOP_HOME/bin:$PATH



编译：compile.sh 编译



配置：elephant.conf 

第一次需要配置JVM参数（自动创建表），后续不需要：

$> vim ./app-conf/elephant.conf 

jvm_props=" -Devolutionplugin=enabled -DapplyEvolutions.default=true"



启动 MR 的jobhistory



前端安装

```shell
sudo yum install npm
sudo npm install -g bower
cd web; bower install; cd ..
```



## 开发