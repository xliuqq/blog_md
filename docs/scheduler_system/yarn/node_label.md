[toc]

# 节点标签

将CPU和GPU节点隔离，且GPU分区不能被CPU（default）共享。

为什么要进行分区，而不是只通过百分比设置队列？

1. CPU和GPU的资源配置，可以相互独立；

2. CPU作业不会影响到GPU的作业，特别对于AI作业需要迭代的情况；

 

## 作业提交时的分区限制

对于GPU作业，通过spark封装提交到yarn上，并且**通过节点标签的限制，将executor限制到GPU分区上运行**。

同一个队列，配CPU和GPU节点的容量，如何**限制CPU作业运行到GPU分区上**？

> Specify required node label in resource request, it will only be allocated when node has the same label. **If no node label requirement specified, such Resource Request will only be allocated on nodes belong to DEFAULT partition.**

通过官方说明，除了AI作业（GPU label）外，其它作业可以**通过不指定标签，将作业直接限制在default分区**。

