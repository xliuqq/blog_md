# Namespace

进程间隔离，linux内核提供`PID、Mount、UTS、IPC、Network、User` Namespace。



## UTS NameSpace

主机/域名（uts）：UNIX Time-sharing System，在网络上视作独立节点；

## IPC NameSpace

进程间通信（ipc）：Inter-Process Communication，包括信号、消息队列和共享内存。





## User NameSpace

> Linux 3.8 新增的一种 namespace，用于隔离安全相关的资源，包括 **user IDs and group IDs**，**keys**, 和 **capabilities**。同样一个用户的 user ID 和 group ID 在不同的 user namespace 中可以不一样(与 PID nanespace 类似)。
>
> 换句话说，**一个用户可以在一个 user namespace 中是普通用户，但在另一个 user namespace 中是超级用户**。

隔离安全相关的标识符(identifiers)和属性（attributes），包括用户ID、用户组ID、root目录、key（密钥）以及特殊权限。

cloneh函数的`CLONE_NEWNS`标志



## Net NameSpace

网络资源的隔离：网络设备，IPV4/IPV6，IP路由表，防火墙，/proc/net目录，/sys/class/net目录，套接字等；

一个**物理的网络设备最多存在一个network namespace**中，可以通过**创建`veth pair`（虚拟网络设备对，类似管道，双向数据传输）在不同的network namespace间创建通道**，达到通信目的。

在建立`veth pair`前，新旧namespace通过**管道**进行通信。

## PID NameSpace

不同NS下的进程可以有相同的PID，内核为所有NS维护树形结构；

- PID隔离，需要配合MOUNT隔离，将`/proc`文件系统重新挂载；

### Init进程

- 进程号为1的进程需要负责孤儿进程的回收，因此Docker容器运行多个进程时，最先启动的命令进程应该时具有资源监控和回收等管理能力，如`bash`；
- Init进程具备信号屏蔽，如果未编写处理某个信号的代码逻辑，则同一个Pid namespace下的进程（即使具有超级权限）发给Init进程的信号会被屏蔽，防止被误杀；
- Namespace树中父节点进程发送的信号，只有`SIGKILL`和`SIGSTOP`会被处理，父节点进程有权终止子节点进程；
- 一旦init进程被销毁，同一PID namespace中的其它进程都会受到SIKILL信号而被销毁。



## Mount NameSpace

### 挂载传播（Mount Propagation）

挂载对象的关系：共享关系，从属关系，私有关系（不可绑定挂载）

- 共享挂载：`mount --make-shared <mount-object>`
- 从属挂载：`mount --make-slave <mount-object>`
- 私有挂载（默认）：`mount --make-private <mount-object>`

挂载状态只可能为以下（K8s的Mount propagation概念对应）：

- 共享挂载（shared）：双向传播，一个挂载对象的挂载事件会传播到另一个挂载对象；
- 从属挂载（slave）：单向传播，反之不行；
- 共享/从属挂载（shared/slave）：具备前两种；
- 私有挂载（private）：各mount namespace之间相互隔离；
- 不可绑定挂载（unbindable）：不可让其它mount namespace 挂载；

### 原理

Mount Namespace跟其它Namespace使用不同的地方：

- 对容器进程视图的改变一定要伴随着**挂载操作**才能生效。

即先clone指定`CLONE_NEWNS`标志，然后在执行/bin/bash前，先挂载目录。

```c
#define _GNU_SOURCE
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#define STACK_SIZE (1024 * 1024)
static char container_stack[STACK_SIZE];

char* const container_args[] = {"/bin/bash", NULL};

int container_main(void *args) {
    printf("Container - inside the container!\n");
    // 以tmpfs（内存盘）格式重新挂载/tmp目录
    mount("none", "tmp", "tmpfs" 0, "");
    execv(container_args[0], container_args);
    printf("Something wrong!\n");
    return 1;
}

int main() {
    printf("Parent - start a container!\n");
    int container_pid = clone(container_main, container_stack + STACK_SIZE, CLONE_NEWNS | SIGCHLD, NULL);
    waitpid(container_pid, NULL, 0);
    printf("Parent - container stopped@\n");
    return 0;
}
```

编译执行：

```bash
# 编译
gcc -o mount_test mount_test.c`
# 执行，进行新的`bash`环境，
./mount_test
# 查看/tmp，发现是个空目录，重新挂载生效
ls /tmp
```

### 容器镜像（rootfs）

> 挂载在容器根目录上用来为容器进程提供隔离后执行环境的文件系统。

为了让容器的根目录看起来更“真实”，一般会在**容器的根目录下挂载一个完整操作系统的文件系统**。

**新的容器，看到的文件系统是独立的隔离环境，而不是继承宿主机的文件系统，如何实现？**

- 在容器进程启动前，挂载它的整个目录'/'；
- `chroot`：change root file system，改变进程的根目录到指定位置。
  - Docker先使用`pivot_root`系统调用，系统不支持再使用`chroot`。

```shell
# 执行命令（/bin/bash），并将$HOME/test目录作为/bin/bash进程的根目录
chroot $HOME/test /bin/bash
```

## Linux C API

### 创建NameSpace

Linux创建新进程的可选参数，比如`PID Namespace : CLONE_NEWPID`

```c
// 新创建的进程会”看到“全新的进程空间，在这个进程空间里，其PID为1，通过ps等看不到宿主机的进程空间
// 但是在宿主机里，其PID还是真实的数值，比如100
int pid = clone(main_function, stack_size, CLONE_NEWPID | SIGCHLD, NULL);
```

`unshare`：在原先进程上实现隔离，不需要创建新的进程。

- PID：原先进程不进入新的PID NameSpace，后续创建的**子进程**才进入（即**新namespace中的init进程**）；
- 其它名空间：原进程直接进入新的Namespace；

### 加入NameSpace

进程的每种Linux NameSpace在对应的`/proc/[进程号]/ns`下有对应的虚拟文件，链接到真实的NameSpace文件上。

`setns`系统调用可以加入某个NameSpace中：

- 同`unshare`，原进程不入新的NS（仅PID），创建子进程才入NS；

`exec`族的函数(`execv, execvp, execl, execlp`)执行命令。

- `exec**`：C程序立即被实际命令替换，即`exec**`之后的代码不会执行，如果需要控制，可以使用`fork-exec`机制；命令执行失败，返回-1；

示例如下：`./setns-test /proc/27342/uts /bin/bash`

```c++
#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char* argv[]) {
    int fd = open(argv[1], O_RDONLY);  /* 获取namespace文件描述符 */
    if (setns(fd, 0) == -1) {          /* 加入新的namespace */
        errExit("setns");
    }
    execvp(argv[2], &argv[2]);		   /* 执行程序 */
    errExit("execvp");				   /* 命令执行成功时，此行不会执行 */
}
```

