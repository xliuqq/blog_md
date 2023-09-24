# 基础

> [GNU CoreUtils](https://github.com/coreutils/coreutils)

## SElinux

安全增强型 Linux（Security-Enhanced Linux），一个 Linux 内核模块，也是 Linux 的一个安全子系统。

SELinux 属于MAC（Mandatory Access Control ）的具体实现。MAC机制的特点在于，资源的拥有者，并不能决定谁可以接入到资源。具体决定是否可以接入到资源，是基于安全策略。而安全策略则是有一系列的接入规则组成，并仅有特定权限的用户有权限操作安全策略。

## 基本信息

### 内核

uname -r 或 cat /proc/version

### HostName

 集群配置中需要注意

***/etc/hosts*** 配置主机名到IP的映射

***/etc/hostname*** 配置主机名，可以通过hostnamectl修改

```shell
sudo hostnamectl set-hostname <newhostname>
```



## Mount

### 挂载命令

默认所有的挂载状态都是私有的。

```shell
mount -t tmpfs -o size=2048M tmpfs /mnt/ramdisk
```

mount 命令用来挂载文件系统。其基本命令格式为：

> **mount -t type [-o options] device dir**

**device**：指定要挂载的设备，比如磁盘、光驱等。
**dir**：指定把文件系统挂载到哪个目录。
**type**：指定挂载的文件系统类型，一般不用指定，mount 命令能够自行判断。
**options**：指定挂载参数，比如 ro 表示以只读方式挂载文件系统。 

### 绑定挂载

```bash
# --bind 将两个目录连接起来，将/host挂载到/test上，对/test的访问和修改都是在/host上
$ mount --bind /host /test
```

### 联合挂载

```shell
# 将A和B目录挂载到一个公共的目录C上
$ mount -t aufs -o dirs=./A:./B none ./C
```



### /etc/fstab 自动挂载配置文件



### 查看已经挂载的目录

```
mount -l
```



## 虚存分配策略

 overcommit_memory是一个内核对内存分配的一种策略。 具体见`/proc/sys/vm/overcommit_memory`下的值

- 0， 表示内核将检查是否有足够的可用物理内存供应用进程使用；如果有，内存申请允许；否则，内存申请失败，并把错误返回给应用进程；

- 1， 表示内核允许分配所有的物理内存，而不管当前的内存状态如何；

- 2， 表示内核**拒绝等于或者大于总可用 swap** **大小以及overcommit_ratio指定的物理RAM**比例的内存请求

  - overcommit_ratio， 当overcommit_memory=2的时候，它一般是代表的是系统中总的内存的百分比

  ```shell
  # 默认为0
  cat /proc/sys/vm/overcommit_memory
  # 默认为50
  cat /proc/sys/vm/overcommit_ratio
  ```


### 虚拟内存限制

 `CommitLimit = SwapTotal + MemTotal * overcommit_ratio`

 总的虚拟内存 = 总的交换分区 + 总的物理内存 * overcommit_ratio

```shell
# 查看系统的内存分配上限CommitLimit和已分配内存大小Committed_As
cat /proc/meminfo | grep Commit
```



## 清空系统缓存

 ```shell
echo 1 > /proc/sys/vm/drop_caches
# 通过free -g 查看是否缓存被清空
 ```

0 – 不释放

1 – 释放页缓存

2 – 释放文件节点缓存和目录项缓存

3 – 释放所有缓存



## 用户共享文件配置

### /etc/skel/目录

/etc/skel/目录是用来存放新用户配置文件的目录，当我们**添加新用户的时候，这个目录下的所有文件会自动被复制到新添加的用户的家目录下**。 

- 初始统一的环境配置；
- **数据共享**；



## 文件和路径

### 链接

符号链接：`ln -s src dst`

- 不同的inode，删除源文件，符号链接失效；

硬链接：`ln src dst`

- 指向同一个inode，一个文件有多个路径名（防止误删），源文件删除后，硬链接仍可读取；

### 内容清空

`ls`文件大小为0：

`cat /dev/null > filename`

### Unzip

批量解压

```shell
ls *.zip | xargs -n1 unzip -o -P infected
```

### find

find 的结果**默认按文件/目录的创建顺序排序**，**不要依赖find的结果顺序**，需要顺序的时候加上sort；

### 文件类型

`file` 命令将帮助你确定你所处理的文件类型。

```shell
$ file /bin/ls
/bin/ls: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.32, BuildID[sha1]=94943a89d17e9d373b2794dcb1f7e38c95b66c86, stripped
```

### 获取路径的最后文件名

`basename /root/a.tar.gz` 返回 `a.tar.gz`。

### 获取当前脚本的绝对路径

- 常见错误命令

  - pwd : **获取当前工作目录**， 不是脚本所在的目录，取决于脚本是 `./script.sh` 还是`./a/script.sh`；
  - $0 : 
    - 使用一个文件调用bash，那$0的值，是那个文件的名字(没说是绝对路径)
    - 使用-c选项启动bash的话，真正执行的命令会从一个字符串中读取，字符串后面如果还有别的参数的话，使用从$0开始的特殊变量引用(跟路径无关了)
    - 除此以外，$0会被设置成调用bash的那个文件的名字(没说是绝对路径)

- 正确命令

  ```shell
  basepath=$(cd `dirname $0`; pwd)
  ```

### sudo免密码执行文件

编辑 `/etc/sudoers`

```shell
usera ALL=(root) NOPASSWD:/opt/sudobin2/commoninstall.sh
```

## 用户

Linux kernel 2.6之后，**最大的用户/用户组数目超过4 billion**；

`stat -c %U file`：显示file所属用户的名称

### id

显示当前用户名，`/usr/bin/id -u -n`

### useradd

```shell
$ useradd [-mMnr][-c <备注>][-d <登入目录>][-e <有效期限>][-f <缓冲天数>][-g <群组>][-G <群组>][-s <shell>][-u <uid>][用户帐号]
```

- -m  自动建立用户的登入目录（默认）；
- -M  不要自动建立用户的登入目录；
- -n   取消建立以用户名称为名的群组；
- -g 指定用户的群组；
- -r  建立系统帐号；
- -u 指定用户id，0~499默认是保留给系统用户账号使用的，所以该值必须大于499

### userdel

-f	强制删除用户，即使用户当前已登录

-r	删除用户的同时，删除与用户相关的所有文件

## 用户组

`groupadd group_test`

设置某个用户所在的用户组

`usermod -g 用户组 用户名` 



## 进制转换

### hexdump

**二进制转 ASCII、十进制、十六进制或八进制显示文件内容。**

- -**n length**：格式化输出文件的前length个字节
- -**C**：输出规范的十六进制和ASCII码
- **-b**：单字节八进制显示
- **-c**：单字节字符显示
- **-d**：双字节十进制显示
- **-o**：双字节八进制显示
- **-x**：双字节十六进制显示
- **-s**：从偏移量开始输出

```shell
$ hexdump -C /bin/ls | head
00000000  7f 45 4c 46 02 01 01 00  00 00 00 00 00 00 00 00  |.ELF............|
00000010  02 00 3e 00 01 00 00 00  d4 42 40 00 00 00 00 00  |..>......B@.....|
00000020  40 00 00 00 00 00 00 00  f0 c3 01 00 00 00 00 00  |@...............|
00000030  00 00 00 00 40 00 38 00  09 00 40 00 1f 00 1e 00  |....@.8...@.....|
```

### printf

```shell
# 8进制转16进制
$ printf %d 089
# 10进制转8进制
$ printf %o 123
```



## 字符串相关

### 多行字符输入

```bash
# << 知道其后面输入的分界符，当再次处理分界符时，两个分界符中间的部分将作为标准输入
cat > file << EOF
contents
EOF
```

### grep

- **-x**：匹配多行
- **-v**：不匹配的项
- **-F**：指定pattern，每行一个pattern

示例：寻找在文件B中出现的，但是文件A中未出现的行

```shell
# 直接文件操作
cat B | grep -x -v -F “$(cat A)”
# 字符串可以是以下形式
C_A=$(cat A)
C_B=$(cat B)
echo "$C_B" | grep -x -v -F echo "$C_A"
```

### awk

```bash
awk [-F 字段分隔符]  'BEGIN{actions} /pattern1/{actions1} /pattern2/{actions2} END{actions}'   input_file
# awk -F ':' '{print $2}' a.txt
```

- 若不指定字段分隔符，则使用环境变量的IFS的值（通常为空格）
- 其中 `BEGIN{actions}` 和 `END{actions}` 是可选的；
- 相关变量
- 字段变量： `$0`，​`$1`，​`$2` ...
- `$NF` : 最后一个变量
  - ARGC:命令行参数个数；ARGV:命令行参数数组

awk 和 sed：

- awk适合按列（域）操作，sed适合按行操作
- awk适合对文件的抽取整理，sed适合对文件的编辑

### sed

> sed不管匹配没有匹配到之后后的状态返回码都是0

主要是以行为单位进行处理，可以将数据行进行替换、删除、新增、选取等特定工作，格式如下：

```shell
$ sed [-nefri] ‘command’ 输入文本   

# sed -i 's/aa/bb/g' in.txt
```

常用选项：

- `-n`∶安静模式，则只有经过sed 特殊处理的那一行(或者动作)才会被列出来。
- `-e`∶直接在指令列模式上进行 sed 的动作编辑；
- `-f filename `∶直接将 sed 的动作写在一个文件内；
- `-r`：支持正则语法；
- `-i`∶直接修改读取的档案内容，而不是由屏幕输出。    

常用命令comond：

- `a \str `  ∶追加行，在匹配行后面追加`str`并换行，`$`表示最后一行；
- c`\ str ` ∶替换行；
- `d` ∶删除匹配行；
- `i`  ∶插入行，在匹配行前面；
- `p` ∶列印，亦即将某个选择的资料印出。通常 p 会与参数 sed -n 一起运作；
- `s` ∶替换字符串，将匹配的字符串替换掉；

如何匹配行：

```shell
# 行数匹配，在第一行到最后一行，每行后面添加add one新的一行
$ sed '1,$a \add one' input.txt
# 字符串匹配，包含first字符串行 后面加上add one新的一行
$ sed '/first/a \add one' input.txt
# 正则字符串匹配，包含 以ha开头,day结尾的行 后面加上add one新的一行
$ sed '/^ha.*day$/a \add one' input.txt
```

获取匹配内容

- `\(.*\)`匹配的内容，可用`\1`的形式进行使用；

```shell
cat mail.xml | sed 's/.*<title>\(.*\)<\/title.*<author><name>\([^<]*\)<\/name><email>\([^<]*\).*/Authot: \2 [\3] \nSubject: \1\n/' 
```

## 磁盘

### 获取扇区大小

```shell
$ fdisk -l
```

### 获取磁盘块的大小

```
$ stat /boot/|grep “IO Block”
```

### 操作系统获取页（Page）的大小

```
$ getconf PAGE_SIZE
```



## 网络相关

### 查看可用的端口号范围

```shell
$ cat /proc/sys/net/ipv4/ip_local_port_range
```

### 查看带宽大小

`ethtool`

### 查看网桥和端口连接信息

`brctl show`

### netstat

格式：

```shell
$ netstat -a -p -n
```

- `-a (all)`显示所有选项，默认不显示LISTEN相关
- `-t (tcp)`仅显示tcp相关选项
- `-u (udp)`仅显示udp相关选项
- `-n` 拒绝显示别名，能显示数字的全部转化成数字。
- `-l `仅列出有在 Listen (监听) 的服務状态
- `-p` 显示建立相关链接的程序名
- `-r` 显示路由信息，路由表
- `-e` 显示扩展信息，例如uid等
- `-s` 按各个协议进行统计
- `-c` 每隔一个固定时间，执行该netstat命令。

netstat结果信息分两类

- Active Internet connections，有源**TCP连接**，

  ```
  Proto Recv-Q Send-Q LocalAddress ForeignAddress State
  tcp 0 2 210.34.6.89:telnet 210.34.6.96:2873 ESTABLISHED
  tcp 296 0 210.34.6.89:1165 210.34.6.84:netbios-ssn ESTABLISHED
  tcp 0 0 localhost.localdom:9001 localhost.localdom:1162 ESTABLISHED
  tcp 0 0 localhost.localdom:1162 localhost.localdom:9001 ESTABLISHED
  tcp 0 80 210.34.6.89:1161 210.34.6.10:netbios-ssn CLOSE
  ```

- Active Unix domain sockets，有源**Unix域套接口**，**RefCnt表示连接到本套接口上的进程号**

  ```
  Proto RefCnt Flags Type State I-Node Path
  unix 1 [ ] STREAM CONNECTED 16178 @000000dd
  unix 1 [ ] STREAM CONNECTED 16176 @000000dc
  unix 9 [ ] DGRAM 5292 /dev/log
  unix 1 [ ] STREAM CONNECTED 16182 @000000df
  ```



### iptable

```shell
$ iptables -t 表名 <-A/I/D/R> 规则链名 [规则号] <-i/o 网卡名> -p 协议名 <-s 源IP/源子网> --sport 源端口 <-d 目标IP/目标子网> --dport 目标端口 -j 动作
```

选项参数

- `-t<表>`：指定要操纵的表；
- `-A`：向规则链中添加条目；
- `-D`：从规则链中删除条目；
- `-I`：向规则链中插入条目；
- `-R`：替换规则链中的条目；
- `-L`：显示规则链中已有的条目；
- `-F`：清除规则链中已有的条目；
- `-Z`：清空规则链中的数据包计算器和字节计数器；
- `-N`：创建新的用户自定义规则链；
- `-P`：定义规则链中的默认目标；
- `-p`：指定要匹配的数据包协议类型；
- `-s`：指定要匹配的数据包源 ip 地址；
- `-d`：指定要匹配的数据包目标 ip 地址；
- `--sport num` 匹配来源端口号；
- `--dport num` 匹配目标端口号；
- `-j<目标>`：指定要跳转的目标；
- `-i<网络接口>`：指定数据包进入本机的网络接口，eth0；
- `-o<网络接口>`：指定数据包要离开本机所使用的网络接口。

表名包括：

- **raw**：高级功能，如：网址过滤。
- **mangle**：数据包修改（QOS），用于实现服务质量。
- **net**：地址转换，用于网关路由器。
- **filter**：包过滤，用于防火墙规则。（default）

规则链名包括：

- **INPUT链**：处理输入数据包。
- **OUTPUT链**：处理输出数据包。
- **FORWARD链**：处理转发数据包。
- **PREROUTING链**：用于目标地址转换（DNAT）。
- **POSTOUTING链**：用于源地址转换（SNAT）。

动作包括：

- **accept**：接收数据包。
- **DROP**：丢弃数据包。
- **REDIRECT**：重定向、映射、透明代理。
- **SNAT**：源地址转换。
- **DNAT**：目标地址转换。
- **MASQUERADE**：IP伪装（NAT），用于ADSL。
- **LOG**：日志记录。

## 进程相关

### nohup

> nohup command > myout.file 2>&1 &  

& ： 指在后台运行，但当用户退出(挂起)的时候，命令自动也跟着退出

nohup ： 不挂断的运行，注意并没有后台运行的功能。



### kill

`kill -Signal pid`

kill：

- kill 会默认传15代表的信号为**SIGTERM**，这是告诉进程你需要被关闭，请自行停止运行并退出，进程可以清理缓存自行结束，也可以拒绝结束

kill -HUP 或 kill -1：

- 根据约定，当您发送一个挂起信号（信号 1 或 HUP）时，大多数服务器进程（所有常用的进程）都会进行复位操作并重新加载它们的配置文件

**kill -3**：

- **可以打印进程各个线程的堆栈信息，不会杀进程**，kill -3 pid 后文件的保存路径为：**/proc/${pid}/cwd**，文件名为：antBuilderOutput.log

kill -9：

- SIGKILL，表示进程被终止，需要立即退出，强制杀死该进程，这个信号不能被捕获也不能被忽略
- 对于 Java 进程，其 ShutDownHook 不会被执行；

kill -0：

- 不发送信号，但是仍然进行错误检查，可用作检查进程id是否存在；

组合信息杀进程：

- 根据线程名kill：ps -au xliu | grep gedit | awk '{print $1}' | xargs kill -9

- 根据jps来kill ： jps | grep JobManage | awk '{print $1}' | xargs kill -9

杀掉远程的进程不一样，上面的awk，需要转义。

`jps | grep JobManage | awk '{print \$1}' | xargs kill -9`



### 父子进程退出

进程意外退出时，如何将主进程创建的子进程终止，有两种做法。

- 一种是将是将主进程中创建的子进程信息保存，使用信号处理机制，在主进程收到终止信号SIGTERM时，保存的子进程信息terminate，之后主进程退出；
- 另一种是更加直接，通过进程组id（`getpgid(pid)`)将整个进程组中的进程杀死。



## 判断命令是否存在

- which，非内置命令，依赖于平台，

- type、hash 或 command，内置命令；再判断 `$?`的值 

``` shell
if command -v foo >/dev/null 2>&1; then echo 0; else echo 1; fi
type foo >/dev/null 2>&1
hash foo 2>/dev/null
```



## crontab

- crontab -l 列出定时任务，crontab -e 编辑定时任务；

- 定时任务格式：f1 f2 f3 f4 f5 program

  - f1表示分钟，f2表示小时，f3表示一月中第几日，f4表示月份，f5表示星期中第几天；
  - f1为*/n表示每个n分钟执行一次；

## Yum

### 代理

proxy=http://代理服务器IP地址:端口号

proxy_username=代理服务器用户名

proxy_password=代理服务器密码



## [调试相关命令](./binutils.md)

> 工具命令
>
> [Coreutils - GNU core utilitieshttps://www.gnu.org/software/binutils/)

