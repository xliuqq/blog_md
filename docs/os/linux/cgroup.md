# CGroup

cgroup，其本身的作用只是任务跟踪。但其它系统（比如cpusets，cpuacct），可以利用cgroup的这个功能实现一些新的属性，比如统计或者控制一个cgroup中进程可以访问的资源。举个例子，cpusets子系统可以将进程绑定到特定的cpu和内存节点上。

**control groups 理解为 controller （system resource）（for）（process）groups**，以一组进程为目标进行系统资源分配和控制。

## 功能

- 资源限制：任务的资源总额进行限制；
- 优先级分配：通过分配的CPU时间片数量及磁盘IO带宽大小，相当于控制任务优先级；
- 资源统计：统计系统的资源使用量，如CPU使用时长、内存用量等，适用于计费；
- 任务控制：对任务执行进行挂起、恢复等操作；

## 基本概念

### Task

系统进程或线程

### cgroup 

控制组，它提供了一套机制用于**控制一组特定进程对资源的使用**。

**关联一组task和一组subsystem的配置参数**。一个task对应一个进程，cgroup是资源分片的最小单位。

### subsystem

子系统，一个通过cgroup提供的工具和接口来管理进程集合的模块。一个子系统就是一个典型的“资源控制器”，用来**调度资源或者控制资源使用的上限**。其实**每种资源就是一个子系统**。子系统可以是以进程为单位的任何东西，比如虚拟化子系统、内存子系统。

### hierarchy

层级树，多个cgroup的集合，这些集合构成的树叫hierarchy。

可以认为这是一个资源树，**附着在这上面的进程可以使用的资源上限必须受树上节点（cgroup）的控制**。hierarchy上的层次关系通过cgroupfs虚拟文件系统显示。

系统允许多个hierarchy同时存在，每个hierachy包含系统中的部分或者全部进程集合。

### cgroupfs

用户管理操纵cgroup的主要接口：

- 通过在cgroupfs文件系统中创建目录，实现cgroup的创建；
- 通过向目录下的属性文件写入内容，设置cgroup对资源的控制；
- 向task属性文件写入进程ID，可以将进程绑定到某个cgroup，以此达到控制进程资源使用的目的；
- 列出cgroup包含的进程pid。



## subsystem配置参数介绍

### blkio - BLOCK IO 资源控制

限额类是主要有两种策略：

- 一种是基于完全公平队列调度（CFQ：Completely Fair Queuing ）的按权重分配各个 cgroup 所能占用总体资源的百分比，好处是当资源空闲时可以充分利用，但只能用于最底层节点 cgroup 的配置；

- 另一种则是设定资源使用上限，这种限额在各个层次的 cgroup 都可以配置，但这种限制较为生硬，并且容器之间依然会出现资源的竞争。

### cpu - CPU 资源控制

CPU 资源的控制也有两种策略：

- 一种是完全公平调度 （CFS：Completely Fair Scheduler）策略，提供了限额和按比例分配两种方式进行资源控制；
- 另一种是实时调度（Real-Time Scheduler）策略，针对实时进程按周期分配固定的运行时间。配置时间都以微秒（µs）为单位，文件名中用us表示。

### cpuacct - CPU 资源报告

### cpuset - CPU 绑定

### device - 限制 task 对 device 的使用

### freezer - 暂停 / 恢复 cgroup 中的 task

### memory - 内存资源管理

- `memory.limit_in_bytes`：物理内存使用限制
- `memory.soft_limit_in_bytes`：限制的内存软额度
- `memory.memsw.limit_in_bytes`：限制swap+memory的大小

- `memory.oom_disable`的值来设置内存超出设定值时是操作系统kill进程还是休眠进程。
- `memory.swappiness` 文件的值修改为 0，进程不使用Swap分区（重启失效）。

### net_prio — 网络设备优先级

### net_cls — 配合tc进行网络限制



## cgroup操作准则与方法

### 准则

#### 一个hierarchy可以有多个subsystem

mount 的时候hierarchy可以attach多个subsystem



#### 一个已经被挂载的 subsystem 只能被再次挂载在一个空的 hierarchy 上

已经mount一个subsystem的hierarchy不能挂载一个已经被其它hierarchy挂载的subsystem



#### 每个task只能在同一个hierarchy的唯一一个cgroup里

不能在同一个hierarchy下有超过一个cgroup的tasks里同时有这个进程的pid



#### 子进程在被fork出时自动继承父进程所在cgroup，但是fork之后就可以按需调整到其他cgroup



### 方法

#### 层级（Hierarchy ）

```shell
# 创建了一个Hierarchy
$ mkdir /sys/fs/cgroup/cpu_and_mem 
# 挂载生效，并附加了cpu和cpuset两个子系统。
$ mount -t cgroup -o cpu,cpuset cpu_and_mem /sys/fs/cgroup/cpu_and_mem/ 
# 附加memory子系统
$ mount -t cgroup -o remount,cpu,cpuset,memory cpu_and_mem /sys/fs/cgroup/cpu_and_mem/
# 剥离memory子系统
$ mount -t cgroup -o remount,cpu cpu_and_mem /sys/fs/cgroup/cpu_and_mem/
# 删除层级
$ umount /sys/fs/cgroup/cpu_and_mem/
```

#### Control Group

创建Hierarchy的时候其实已经创建了了一个Control Group叫做root group，其表现形式就是一个目录，一个目录就是一个Control Group。

```shell
# -t指定的用户或者组对tasks文件的拥有权
# -a指定的用户或者组对其它文件的拥有权
# -g 指定要附加的子系统（必须是层级拥有的）和Control Groups路径
$ cgcreate -t uid:gid -a uid:gid -g subsystems:path

# cgcreate会根据-g后面指定的子系统，找到包含这些子系统的所有Hierarchy，并为其创建control group
$ cgcreate -g cpuset:/subgroup

# 删除的时候也要指定子系统，会根据子系统找到所有的Hierarchy 中的指定Control Group来删除
$ cgdelete cpuset,memory:/subgroup
```



## 使用示例

### Linux是否启用cgroup

```shell
$ uname -r
# 4.18.0-24-generic
$ cat /boot/config-4.18.0-24-generic | grep CGROUP
# 对应的CGROUP项为“y”代表已经打开linux cgroups功能。
```

Centos默认已经为**每个子系统创建单独Hierarchy** ，进行查看：

```shell
$ mount -t cgroup
tmpfs /sys/fs/cgroup tmpfs ro,seclabel,nosuid,nodev,noexec,mode=755 0 0
cgroup /sys/fs/cgroup/systemd cgroup rw,seclabel,nosuid,nodev,noexec,relatime,xattr,release_agent=/usr/lib/systemd/systemd-cgroups-agent,name=systemd 0 0
cgroup /sys/fs/cgroup/hugetlb cgroup rw,seclabel,nosuid,nodev,noexec,relatime,hugetlb 0 0
cgroup /sys/fs/cgroup/cpu,cpuacct cgroup rw,seclabel,nosuid,nodev,noexec,relatime,cpuacct,cpu 0 0
cgroup /sys/fs/cgroup/perf_event cgroup rw,seclabel,nosuid,nodev,noexec,relatime,perf_event 0 0
cgroup /sys/fs/cgroup/net_cls,net_prio cgroup rw,seclabel,nosuid,nodev,noexec,relatime,net_prio,net_cls 0 0
cgroup /sys/fs/cgroup/cpuset cgroup rw,seclabel,nosuid,nodev,noexec,relatime,cpuset 0 0
cgroup /sys/fs/cgroup/memory cgroup rw,seclabel,nosuid,nodev,noexec,relatime,memory 0 0
cgroup /sys/fs/cgroup/blkio cgroup rw,seclabel,nosuid,nodev,noexec,relatime,blkio 0 0
cgroup /sys/fs/cgroup/devices cgroup rw,seclabel,nosuid,nodev,noexec,relatime,devices 0 0
cgroup /sys/fs/cgroup/pids cgroup rw,seclabel,nosuid,nodev,noexec,relatime,pids 0 0
cgroup /sys/fs/cgroup/freezer cgroup rw,seclabel,nosuid,nodev,noexec,relatime,freezer 0 0
```

### 显示CPU使用比例

在目录/sys/fs/cgroup/cpu`下创建文件夹，就创建了一个control group

root 用户，写一个死循环的代码，将CPU使用率通过cgroup限制在 20%

```shell
# 在/sys/fs/cgroup/cpu/创建进程名
$ mkdir /sys/fs/cgroup/cpu/test
# 创建成功后，自动创建cpu配置文件
$ cd /sys/fs/cgroup/cpu/test
$ ls
# cgroup.clone_children  cgroup.procs  cpuacct.stat  cpuacct.usage  cpuacct.usage_percpu  # cpu.cfs_period_us  cpu.cfs_quota_us  cpu.shares  cpu.stat  notify_on_release  tasks

$ cat cpu.cfs_quota_us
# -1
# 如果cgroup只读，执行 mount -o remount,rw /sys/fs/cgroup
# 限制进程只占用 20%的CPU
$ echo 100000 > cpu.cfs_period_us
$ echo 20000 > cpu.cfs_quota_us
# 将进程号写入tasks里
$ echo 30167 > tasks

# 删除cgroup (要么停止进程，要么将进程从tasks文件里删除)
rmdir tasks
```

