# [GNU Binutils](https://www.gnu.org/software/binutils/)

The GNU Binutils are a collection of binary tools. The main ones are:

- **ld** - the GNU linker.
- **as** - the GNU assembler.
- **gold** - a new, faster, ELF only linker.

But they also include:

- **addr2line** - Converts addresses into filenames and line numbers.
- **ar** - A utility for creating, modifying and extracting from archives.
- **c++filt** - Filter to demangle encoded C++ symbols.
- **dlltool** - Creates files for building and using DLLs.
- **elfedit** - Allows alteration of ELF format files.
- **gprof** - Displays profiling information.
- **gprofng** - Collects and displays application performance data.
- **nlmconv** - Converts object code into an NLM.
- **nm** - Lists symbols from object files.
- **objcopy** - Copies and translates object files.
- **objdump** - Displays information from object files.
- **ranlib** - Generates an index to the contents of an archive.
- **readelf** - Displays information from any ELF format object file.
- **size** - Lists the section sizes of an object or archive file.
- **strings** - Lists printable strings from files.
- **strip** - Discards symbols.
- **windmc** - A Windows compatible message compiler.
- **windres** - A compiler for Windows resource files.

## addr2line

> Converts addresses into filenames and line numbers.

**通过 `-g`的debug信息**，获取错误的行号信息。

```shell
# 静态链接
$ addr2line -e backtrace 0x400a3e
# 输出结果类似如下信息，可以看出行号信息
/home/share/work/backtrace/add.c:13

# 动态链接，运行时动态加载，其加载地址不一样，通过/proc/$pid/maps 获取内存和动态链接库
$ addr2line -e libadd.so 0x7f85839fa5c6
??:0
# 根据出错位置的 0x7f0962fb35c6 减去初始的加载地址 0x7f0962fb3000，得到 0x5c6
$ addr2line -e libadd.so 0x5c6
/home/share/work/backtrace/add.c:13
```

## ld

ld（Link eDitor）命令是二进制工具集 [GNU Binutils](https://www.gnu.org/software/binutils/) 的一员，是 GNU 链接器，用于**将目标文件与库链接为可执行文件或库文件**。

- `-e`参数可以指定入口；

```shell
# compile（跟下面的as命令一起 等价于 gcc -c minimal.S)
gcc -S minimal.S > minimal.s
# assemble
as minimal.s -o minimal.o
# 链接为可执行
ld minimal.o -o minimal
```

## as

as 命令是二进制工具集 [GNU Binutils](https://www.gnu.org/software/binutils/) 的一员，是GNU 推出的一款**汇编语言编译器集**，用于将汇编代码编译为二进制代码，它支持多种不同类型的处理器。



## nm

列出对象文件中的符号和地址信息。

```shell
$ nm hello | tail
0000000000600e20 d __JCR_END__
0000000000600e20 d __JCR_LIST__
00000000004005b0 T __libc_csu_fini
0000000000400540 T __libc_csu_init
                 U __libc_start_main@@GLIBC_2.2.5
000000000040051d T main
                 U printf@@GLIBC_2.2.5
0000000000400490 t register_tm_clones
0000000000400430 T _start
0000000000601030 D __TMC_END__
```



## objdump

>  Displays information from object files（从对象文件中显示信息）。

读取二进制或可执行文件，并将汇编语言指令转储到屏幕上。

```shell
$ objdump -d /bin/ls | head

/bin/ls:     file format elf64-x86-64

Disassembly of section .init:

0000000000402150 <_init@@Base>:
  402150:       48 83 ec 08             sub    $0x8,%rsp
  402154:       48 8b 05 6d 8e 21 00    mov    0x218e6d(%rip),%rax        # 61afc8 <__gmon_start__>
  40215b:       48 85 c0                test   %rax,%rax
```



针对`symbol not found`，通过`objdump -T`查看符号链接，如

```shell
$ objdump -T /usr/lib/x86_64-linux-gnu/libdde-file-manager.so.1 | grep hasPartitionTableEv
0000000000000000      DF *UND*  0000000000000000              _ZNK12DBlockDevice17hasPartitionTableEv

# 可以通过 ldd .so + awk + xargs + objdump -T 搜索包含该符号定义的so
$ objdump -T /usr/lib/x86_64-linux-gnu/libudisks2-qt5.so | grep hasPartitionTableEv
0000000000045cb0 g    DF .text  000000000000003d  Base        _ZNK12DBlockDevice17hasPartitionTableEv
```



## readelf

显示有关 ELF 文件的信息。

ELF（ *可执行和可链接文件格式(Executable and Linkable File Format)*）是可执行文件或二进制文件的主流格式，不仅是 Linux 系统，也是各种 UNIX 系统的主流文件格式。

```shell
$ readelf -h /bin/ls
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x4042d4
  Start of program headers:          64 (bytes into file)
  Start of section headers:          115696 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         31
  Section header string table index: 30
```



## strings

打印文件中的可打印字符的字符串。

```shell
$ strings /bin/ls
```

