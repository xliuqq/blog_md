[toc]

# Proc

https://man7.org/linux/man-pages/man5/proc.5.html

虚拟内存：是进程运行时全部内存空间的总和，Java的-Xmx分配的是虚拟内存的空间；



## /proc/self

当前进程的信息，不同线程访问时获取的其线程的信息。



## /proc/[pid]/io

**rchar**：从存储器读取的字节数，是read(2)类似系统调用的字节数之和，**跟实际物理磁盘读取无关**；

**wchar**：写入磁盘的字节数，类似rchar；

**syscr**：读的系统调用

**syscw**：写的系统调用

**read_bytes**：**进程实际读取从存储器中的字节数**

**write_bytes**：进程实际写入从存储器中的字节数



## /proc/stat

```shell
# cpu   user     nice   system   idle     iowait irq softirq steal guest   guest_nice 
 cpu    10132153 290696 3084719 46828483  16683   0   25195   0     175628   0
 cpu0   1393280  32966  572056  13343292  6130    0   17875   0     23933    0
```

时间的单位是`USER_HZ`，

**user**：用户模式下花费的时间；

**nice**：用户模式下以低优先级花费的时间；

**system**：系统模式下花费的时间；

**idle**：空闲的时间

**iowait**：等待I/O的时间，由于以下原因，**不是很可靠**

- 任务等待I/O，而不是CPU等待，CPU空闲时会有其他任务被调度在这个CPU上；
- 在多核CPU上，等待I/O的任务未在任何CPU上运行，因此每个CPU的**iowait**是很难计算的；
- 该字段的值可能会降低



## /proc/[pid]/stat

进程的状态信息



## /proc/[pid]/status

`/proc/[pid]/stat` 和`/proc/[pid]/statm`的大部分信息，可阅读性好。

**VmPeak**：虚拟内存的峰值；

**VmSize**：虚拟内存大小；

**VmRss**：应用程序正在**使用的物理内存的大小**，**不精确**，ps命令的参数rss的值 (rss)；

**VmHWM**：应用程序正在**使用的物理内存的峰值**，**不精确**；

**VmLck**：锁定的物理内存大小，锁住的物理内存不能交换到硬盘 (locked_vm)；

**VmPin**：Pinned的物理内存大小，不能移动；

**VmData**：程序数据段的大小（所占虚拟内存的大小）



# TOP

**VIRT**（即VmSize）：进程使用的虚拟内存总量

**RES**（即VmRss）：进程使用的未被换出的物理内存大小，单位kb 

**SHR**：共享内存大小，单位kb

**%MEM**：进程使用的物理内存百分比
