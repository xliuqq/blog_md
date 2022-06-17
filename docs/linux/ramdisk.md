# RamDisk

虚拟内存盘是通过软件将一部分内存（RAM）模拟为硬盘来使用的一种技术，Alluxio软件就是将 RAMDisk 作为其存储介质。

分为`Ramdisk`, `ramfs`, `tmpfs`：

- Ramdisk 是传统意义上磁盘，可以格式化，然后加载；

  - Linux 2.0/2.2支持，大小固定，不能改变；
  - 内核编译时将Block device中Ramdisk支持选上，

- ramfs和tmpfs是Linux内核2.4支持，大小可以自动增加或减少；

  - ramfs内存文件系统，它处于虚拟文件系统(VFS)层，无需格式化，默认一半内存大小；
  - 可以在mount时限定大小，不会自动增长
  - **ramfs不会用swap**，**tmpfs会使用swap**
  
  

Linux下的挂载：

``` shell
mkdir -p /media/nameme

mount -t tmpfs -o size=2048M tmpfs /mnt/ramdisk

mount -t ramfs -o size=2048M ramfs /mnt/ramdisk
```



性能测试

```shell
# 测试磁盘的写速率
$ time dd if=/dev/zero bs=4096 count=2000000 of=./8Gb.file
2000000+0 records in
2000000+0 records out
8192000000 bytes (8.2 GB) copied, 6.37105 s, 1.3 GB/s

real    0m6.374s
user    0m0.909s
sys     0m5.450s

# 测试磁盘读
$ time dd if=/dev/mapper/VolGroup00-LogVol00 of=/dev/null bs=8k
 2498560+0 records in
 2498560+0 records out
 247.99s real 1.92s user 48.64s system
```

