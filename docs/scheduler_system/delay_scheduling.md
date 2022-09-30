[toc]

# Scheduler

## Gang scheduler

用户提交一个 batch job, 这个batch job 包含100个任务，要不这100个任务全部调度成功，要么一个都调度不成功。这种`all or nothing`调度场景，就被称作:`gang-schedule`。



## Delay Scheduling

TODO：看论文

[2010_eurosys_delay_scheduling](./pdf/eurosys_delay_scheduling.pdf)

Yarn中的调度算法，延迟调度，实现数据本地性



Yarn级别的Delay Scheduler

- spark的executor分配在哪些nodemanager上

Spark内部的Task的Delay Scheduler

- task分配到哪个executor上



思考：TODO（

- 在计算/存储分离场景下，延迟调度还有用么？该用什么调度算法呢？