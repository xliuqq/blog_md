



在splitAssigner对DataSourceTask进行split分配的时候，会考虑数据本地性，**优先分配在其本地的数据给DataSourceTask**，如果没有本地的则会分配一个远程的。该部分代码在**LocatableInputSplitAssigner.getNextInputSplit()**中可以看到。



TODO：

Flink 是根据 TaskId（slot的id）来获取 Split 进行处理，一个slot会处理多个split么？。