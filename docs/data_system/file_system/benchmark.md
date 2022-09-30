# 文件系统的性能测试

## fio（TODO）

源码地址：https://github.com/axboe/fio

### 使用



## NNBench

Hadoop 中有一个专门压测**文件系统元数据**性能的组件叫 NNBench。

局限性：

- 单个测试任务是单线程的，资源利用率低；
-  使用 hostname 作为路径名的一部分，没有考虑同一个主机里多个并发任务的冲突问题，会导致多个测试任务重复创建和删除文件，不太符合大数据工作负载的实际情况；

### 使用

```shell
bin/hadoop jar share/hadoop/mapreduce2/hadoop-mapreduce-client-jobclient-2.3.0-cdh5.0.0-tests.jar nnbench -operation create_write -maps 4 -reduces 2 -bytesToWrite 1 -numberOfFiles 2 -replicationFactorPerFile 3 -readFileAfterOpen true -baseDir /benchmarks/NNBench
```



## dfs-perf

源码地址：