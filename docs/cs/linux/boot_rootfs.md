# 启动

## Bootfs

> 包含 bootloader和 Kernel，bootloader主要是引导加 kernel，Linux刚启动时会加bootfs文件系统，在 Docker镜像的最底层是bootfs.

当bootfs加载完成之后**整个内核就都在内存中**，此时内存的使用权已由 bootfs转交给内核，此时系统也会卸载bootfs。

## Rootfs

> 根文件系统是**内核启动时所mount的第一个文件系统**，内核代码映像文件保存在根文件系统中，而系统引导启动程序会在根文件系统挂载之后从中把一些基本的初始化脚本和服务等加载到内存中去运行。

包含的就是典型 Linux系统中的/dev、/proc、/bin、/etc等标准目录和文件，rootfs就是各种不同的操作系统发行版，比如：Ubuntu,、CentOS。

