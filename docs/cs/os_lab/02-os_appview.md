# 应用视角的操作系统

>**STFW**: Search The Friendly Website.
>
>**RTFM**: Read The Friendly Manual.
>
>应用程序与操作系统沟通的唯一桥梁是**系统调用指令** (例如 x86-64 的 syscall)。

**本讲内容**：**指令序列和高级语言的状态机模型**；回答以下问题：

- 什么是软件 (程序)？
- 如何在操作系统上构造最小/一般/图形界面应用程序？
- 什么是编译器？编译器把一段程序翻译成什么样的指令序列才算 “正确”？

[随堂代码](https://gitee.com/oscsc/oslabcode/tree/master/ch02-os_app_view)

## 汇编代码的状态机模型

> 最小的"hello,world".

**Everything is a state machine**: 计算机 = 数字电路 = 状态机

- 状态 = 内存 *M* + 寄存器 *R*
- 初始状态 = ABI 规定 (例如有一个合法的 %rsp)
- 状态迁移 = 执行一条指令



操作系统上的程序

- 所有的指令都只能计算
  - deterministic: mov, add, sub, call, ...
  - non-deterministic: rdrand, ...
- **syscall 把 (*M*,*R*) 完全交给操作系统**



### 递归转非递归的转换 

> **模拟栈**执行，将非递归转为递归。

详细可以见编程代码中的 [hanoi-nr.c](https://gitee.com/oscsc/oslabcode/blob/master/ch02-os_app_view/hanoi-nr.c)



## 理解编译器

有两种状态机

- 高级语言代码.*c*
  - 状态：栈、全局变量；状态迁移：语句执行
- 汇编指令序列.*s*
  - 状态：(M*,*R*)；状态迁移：指令执行
- **编译器是二者之间的桥梁**：$.s=compile(.c)$



### 编译器的正确性

.*c* 执行中<font color=red>**所有外部观测者可见的行为**</font>，必须在 .*s* 中保持一致

- External function calls (链接时确定)
  - 如何调用由 Application Binary Interface (ABI) 规定
  - 可能包含系统调用，因此不可更改、不可交换
- 编译器提供的 “不可优化” 标注
  - `volatile` [load | store | inline assembly]
- Termination
  - .*c* 终止当且仅当 .*s* 终止

<font color=red>在此前提下，任何翻译都是合法的</font> (例如我们期望更快或更短的代码)





底层系统实现（通过阅读 busybox 的早期版本的源代码）



Trace

 



ToyBoy、BusyBox