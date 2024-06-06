# Shell 编程

I/O标识为标准输入0，标准输出1，标准错误2； > 重定向输出， >> 附加内容；

## bash，source，exec

- bash执行shell脚本，产生**sub shell**（即fork，执行产生新的子进程，执行完后返回到父进程）执行，执行完后不保留子进程的信息；
- **source执行shell脚本，在当前shell环境中执行**；
- exec 执行不产生新的子进程，但是会替换当前的shell，执行完关闭（一般将exec放到shell脚本中），即**exec后面的脚本不会执行**；



sh 参数： **-n 只检查语法错误**，不执行脚本； **-v 运行命令前先显示命令**； **-x 处理命令后显示命令**

- **-e** ：如果某个指令的执行返回非0，则直接返回，不需要通过 `$?`判断返回值
  - 注意跟 grep 的配合（grep匹配不到则返回非0），可以用 `$(ps -ef | { grep "进程标识" || true; })`

## `${}, $()`

在bash中，`$( )`与\` \`（反引号）都是用来作命令替换的。

`$var`与`${var}`是没有区别的，但是用`${ }`会比较精确的界定变量名称的范围

``` shell
PKG_DIR=$(ls)
for apiName in ${PKG_DIR[*]} ; do echo $apiName; done
for apiName in ${PKG_DIR} ; do echo $apiName; done
```



## 基本语法

### Set

```shell
# 表示一旦脚本中有命令的返回值为非0，则脚本立即退出，后续命令不再执行;
set -e 
# 以调试的形式运行，即会打印出每次执行的语句
set -x
# 表示在管道连接的命令序列中，只要有任何一个命令返回非0值，则整个管道返回非0值，即使最后一个命令返回0.
set -o pipefail 
```



### 变量

``` bash
A="hello world"
```

- 等号两边不能有空格；
- `readonly`用于定义只读变量；

### for

``` shell
for var in value_list ; do
      ...
done
```

注意：

```bash
for line in `cat file`; do done
```

用法，**file 中的空格会被当成行分隔符**，需要指定 `IFS=$'\n'` 才可以正常分行

### while

> read line 读取时，如果最后一行没有换行符则无法读取，可以通过以下形式
>
> ```
> while read line || [ -n $line ]
> do
> done < a.txt
> ```

``` shell
while true
do
    command
done

while true; do $command; done;

# 等价于
for (( ; ; ))
```

### if

``` shell
if condition_judgement ; then 
... 
elif condition2; then 
... 
else 
... 
fi
```

### case

``` shell
case experssion in
pattern1)
      ...;;
pattern2)
      ...;;
*)
   ...;;
esac
```

### 数组

``` shell
array_name=(value1 ... valuen)
```

### 参数

- `$?` : 前一个命令的退出状态，正常0，异常非0；
- `$#` : 脚本或函数的参数个数； $$ : Shell本身的PID
- `$0` : 脚本名称； `$1, $2 … `： 脚本或函数的位置参数；
- `$*` : 以 `$1 $2 …` 保存用户传入的位置参数；` $@` : 以 `$1` `$2 …`$n` 形式保存位置参数

### 运算

```shell
# 整形运算： 
((a=$j+$k))  #或 
let a+= 1 #或 
a=`expr $a + 1` #（加号两边有空格），
((i=$j*$k)) #等价 
i=`expr $j \* $k`  
((i=$j\$k))  #等价
i=`expr $j /$ $k`  

echo $(($j + $k)

```

### 函数

```shell
# 先定义，再使用（无括号，参数加空格）  function_name arg_a arg_b
function_name() { 
       ...	# 函数参数同脚本，通过$1，$2表示
}	
# 函数结尾可以用return语句返回执行状态，0表示无错误；
# 函数默认是将标准输出传递出来，不是返回值；或者使用全局变量
```



## 比较

test用来比较数字、字符串、文件状态检查、逻辑判()断等，test condition 或 [ condition ]，两边有空格，条件成立返回0，否则非0
- 字符串： == 或= , != , -z (空), -n(非空) ；
- 整型： -eq , -ne , -gt , -ge , -lt , -le ；
- 逻辑与（-a)或（-o）非（!）；
- 在[]中使用==是bash里的做法, 不符合posix标准；
- =~ 正则表达式匹配
- ${} 变量，或者执行命令；



文件相关判断：

- `-a`：FILE是文件存在则为真；
- `-d`：FILE是目录存在则为真；
- `-e`：FILE存在则为真；