

[toc]

# C/C++ DEBUG

## 异常

### 处理规则

C++中，异常不可以忽略，当异常找不到匹配的catch字句时，会调用系统的库函数terminate()（在头文件中）。

默认情况下，terminate（）函数调用标准C库函数abort（）使程序终止而退出。

当调用abort函数时，程序不会调用正常的终止函数，**全局对象和静态对象的析构函数不会执行**。



### 异常堆栈

#### std::set_terminate

处理未设置异常处理函数的异常（即通过throw抛出的异常）：

- 对于数组越界异常，无法捕获；



#### 异常信号

当程序出现异常时通常伴随着会收到一个由内核发过来的异常信号，如当**对内存出现非法访问时将收到段错误信号SIGSEGV**，然后才退出。

利用这一点，当我们在收到异常信号后将程序的调用栈进行输出，它通常是利用`signal()`函数。



#### backtrace

**出现崩溃退出时把当前调用栈通过终端打印出来并定位问题的方法**。

在Linux上的C/C++编程环境下，我们可以通过如下三个函数来获取程序的调用栈信息。

```c
#include <execinfo.h>

/* Store up to SIZE return address of the current program state in
   ARRAY and return the exact number of values stored.  */
int backtrace(void **array, int size);

/* Return names of functions from the backtrace list in ARRAY in a newly
   malloc()ed memory block.  */
char **backtrace_symbols(void *const *array, int size);

/* This function is similar to backtrace_symbols() but it writes the result
   immediately to a file.  */
void backtrace_symbols_fd(void *const *array, int size, int fd);
```

它们由GNU C Library提供，关于它们更详细的介绍可参考[Linux Programmer’s Manual](http://man7.org/linux/man-pages/man3/backtrace.3.html)中关于backtrack相关函数的介绍。

backtrace需要注意的地方：

- backtrace的实现**依赖于栈指针（fp寄存器）**，在gcc编译过程中任何**非零的优化等级（-On参数）**或加入了栈指针优化参数**-fomit-frame-pointer**后多将**不能正确得到程序栈信息**；
- backtrace_symbols的实现需要符号名称的支持，在**gcc编译过程中需要加入-rdynamic**参数；
- 内联函数没有栈帧，它在编译过程中被展开在调用的位置；
- 尾调用优化（Tail-call Optimization）将复用当前函数栈，而不再生成新的函数栈，这将导致栈信息不能正确被获取。

获取到的堆栈信息如下所示：

```shell
Dump stack start...
backtrace() returned 8 addresses
  [00] ./backtrace(dump+0x1f) [0x400a53]
  [01] ./backtrace(signal_handler+0x31) [0x400b1b]
  [02] /lib/x86_64-linux-gnu/libc.so.6(+0x36150) [0x7f8583672150]
  [03] ./libadd.so(add1+0x1a) [0x7f85839fa5c6]
  [04] ./libadd.so(add+0x1c) [0x7f85839fa5f9]
  [05] ./backtrace(main+0x2f) [0x400a13]
  [06] /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xed) [0x7f858365d76d]
  [07] ./backtrace() [0x400929]
Dump stack end...
Segmentation fault
```

可以看出出错的函数为 `Java_JniTest_callC`，但地址为 0x7f0bfd8d91ab，看不出是哪一行。

#### addr2line

**通过 `-g`的debug信息**，获取错误的行号信息。

##### 静态链接情况下的错误信息分析定位

编译成一个可执行文件并执行的错误，通过`addr2line`命令可以得到：

```shell
# addr2line -e ${exe_name} ${error_address}
$ addr2line -e backtrace 0x400a3e
# 输出结果类似如下信息，可以看出行号信息
/home/share/work/backtrace/add.c:13
```

##### 动态链接情况下的错误信息分析定位

按照上面方法，得不到正确的信息

```shell
$ addr2line -e libadd.so 0x7f85839fa5c6
??:0
```

因为，动态链接库是程序运行时动态加载的而其加载地址也是每次可能都是不一样，如 0x7f0bfd8d91ab，不是一个实际的物理地址（用户空间的程序无法直接访问物理地址），而是经过MMU（内存管理单元）映射过的。

注意到`add+0x1c `描述出错的地方发生在符号add1偏移0x1a处的地方，**获取 add1 在程序中的入口地址再加上偏移量0x1a也能得到正常的出错地址**。

**得到libadd.so的加载地址**

```shell
# 通过/proc/$pid/maps 获取内存和动态链接库的情况
$ cat /proc/$pid/map
# 输出类似的结果
....................................................
7f0962fb3000-7f0962fb4000 r-xp 00000000 08:01 2895572                    /home/share/work/backtrace/libadd.so
7f0962fb4000-7f09631b3000 ---p 00001000 08:01 2895572                    /home/share/work/backtrace/libadd.so
7f09631b3000-7f09631b4000 r--p 00000000 08:01 2895572                    /home/share/work/backtrace/libadd.so
7f09631b4000-7f09631b5000 rw-p 00001000 08:01 2895572                    /home/share/work/backtrace/libadd.so
.....................省略大量无关内容....................
=========>>>catch signal 11 <<<=========
Dump stack start...
backtrace() returned 8 addresses
  [00] ./backtrace(dump+0x1f) [0x400b7f]
  [01] ./backtrace(signal_handler+0x83) [0x400c99]
  [02] /lib/x86_64-linux-gnu/libc.so.6(+0x36150) [0x7f0962c2b150]
  [03] ./libadd.so(add1+0x1a) [0x7f0962fb35c6]
  [04] ./libadd.so(add+0x1c) [0x7f0962fb35f9]
  [05] ./backtrace(main+0x2f) [0x400b53]
  [06] /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xed) [0x7f0962c1676d]
  [07] ./backtrace() [0x400a69]
Dump stack end...
Segmentation fault
```

Maps信息第一项表示的为地址范围如第一条记录中的7f0962fb3000-7f0962fb4000，第二项r-xp分别表示只读、可执行、私有的。

```shell
# 根据出错位置的 0x7f0962fb35c6 减去初始的加载地址 0x7f0962fb3000，得到 0x5c6
$ addr2line -e libadd.so 0x5c6
/home/share/work/backtrace/add.c:13
```

**得到函数add的入口地址再上偏移量来得到正确的地址（推荐）**

1. 利用gcc编译生成的map文件，添加`-Map,add.map`，生成map文件；

   ```shell
   $ gcc -Wl,-Map,add.map -rdynamic add.c -fPIC -shared -o libadd.so 
   # 查看add.map，关键信息如下
   ................................... 
   .text          0x00000000000005ac       0x55 /tmp/ccCP0hNf.o
                   0x00000000000005ac                add1
                   0x00000000000005dd                add
   ...................................
   ```

   add1的地址为0x5ac，然后加上偏移地址0x1a即0x5ac + 0x1a = 0x5c6。

2. 通过nm获取（**推荐，直接对so获取函数入口地址**）

   ```shell
   $ nm -n libJniC.so 
   # 输出类似如下的信息，获取到函数的入口地址
   00000000000010a0 T _Z10printStackv
   000000000000115d T _Z16exceptionHandlerv
   0000000000001256 T Java_JniTest_callC
   00000000000012f8 t _Z41__static_initialization_and_destruction_0ii
   0000000000001340 t _GLOBAL__sub_I_JniTest.cpp
   ```



#### 示例

*add.c*

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
int add1(int num)
{
	int ret = 0x00;
	int *pTemp = NULL;
	
	*pTemp = 0x01;  /* 这将导致一个段错误，致使程序崩溃退出 */
	
	ret = num + *pTemp;
	
	return ret;
}
 
int add(int num)
{
	int ret = 0x00;
 
	ret = add1(num);
	
	return ret;
}
```

*dump.c*

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>	    /* for signal */
#include <execinfo.h> 	/* for backtrace() */
 
#define BACKTRACE_SIZE   16
 
void dump(void)
{
	int j, nptrs;
	void *buffer[BACKTRACE_SIZE];
	char **strings;
	
	nptrs = backtrace(buffer, BACKTRACE_SIZE);
	printf("backtrace() returned %d addresses\n", nptrs);
	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL) {
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}
 
	for (j = 0; j < nptrs; j++)
		printf("  [%02d] %s\n", j, strings[j]);
 
	free(strings);
}
 
void signal_handler(int signo)
{
	
#if 0	
	char buff[64] = {0x00};
	sprintf(buff,"cat /proc/%d/maps", getpid());
	system((const char*) buff);
#endif	
 
	printf("\n=========>>>catch signal %d <<<=========\n", signo);
	printf("Dump stack start...\n");
	dump();
	printf("Dump stack end...\n");
 
	signal(signo, SIG_DFL); /* 恢复信号默认处理 */
	raise(signo);           /* 重新发送信号 */
}
```

 *backtrace.c*

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>	    /* for signal */
#include <execinfo.h> 	/* for backtrace() */
 
extern void dump(void);
extern void signal_handler(int signo);
extern int add(int num);
 
int main(int argc, char *argv[])
{
	int sum = 0x00;
	signal(SIGSEGV, signal_handler);  /* 为SIGSEGV信号安装新的处理函数 */
	sum = add(sum);
	printf(" sum = %d \n", sum);
	return 0x00;
}
```

