# OS Lab 2022

> https://jyywiki.cn/OS/2022/
>
> 参考书：
>
> - [OSTEP] Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau. *[Operating Systems: Three Easy Pieces](http://pages.cs.wisc.edu/~remzi/OSTEP/)*. Arpaci-Dusseau Books, 2018.
> - [CSAPP] Randal E. Bryant and David R. O'Hallaron. *Computer Systems: A Programmer's Perspective* (3ed). Pearson, 2017. (作为手册和参考书查阅)

- [Parallel and Distributed Computation: Numerical Methods](https://web.mit.edu/dimitrib/www/pdc.html)
- 

## 1. 操作系统概述



## 2. 多处理器编程

> **“程序 (甚至是一条指令) 独占处理器执行”** 的基本假设在现代多处理器系统上不再成立。
>
> **不原子、能乱序、不立即可见**

(历史) 1960s，大家争先在共享内存上实现原子性 (互斥)

- 但几乎所有的实现都是错的，直到 [Dekker's Algorithm](https://en.wikipedia.org/wiki/Dekker's_algorithm)，还只能保证两个线程的互斥

现代线程库：

- Pthread 线程库；
- thread C++ 2011 标准；



**编译器做的是静态乱序优化，CPU做的是动态乱序优化。**

```c
// volatile 关键字表示每次内存数据读取都走主存，避免静态乱序优化
volatile 
// 函数表示内存屏障/禁止指令重排序，避免动态乱序优化
__sync_synchronize()
```



### 课程代码

- [课程提供的线程库](./2_multi_thread/thread.h)
- [线程共享内存测试](./2_multi_thread/shm_test.c)
- [线程独立堆栈和范围测试](./2_multi_thread/stack_test.c)
- [多线程竞态测试](./2_multi_thread/atomic_test.c)：单个处理器的乱序执行，编译器的重排序优化
  - `-O`优化：**编译器对内存访问 “eventually consistent” 的处理导致**共享内存作为线程同步工具的失效。
- [多处理器间即时可见性的丧失](./2_multi_thread/mem_ordering_test.c)：CPU的指令重排序优化，内存同步障解决问题
  - 单个处理器把汇编代码“编译”成更小的$\mu ops$，每个$\mu ops$ 都有 Fetch, Issue, Execute, Commit 四个阶段
    - “多发射”：每一周期向池子补充尽可能多的 $\mu op$
    - “乱序执行”、“按序提交”：每一周期 (在不违反编译正确性的前提下) 执行尽可能多的 $\mu op$ 
- [Peterson's 协议](./2_multi_thread/peterson_simple.c):  需要避免指令重排序
  - Sequential 内存模型下 Peterson's Protocol 的 Safety

- [顺序内存模型的状态机检查](./2_multi_thread/model_check/model_check.py)：仅适用于顺序内存模型（即顺序执行指令）



### 参考资料

 [ARM/RISC-V 内存模型](https://research.swtch.com/mem-weak@2x.png)

[X86内存模型](https://research.swtch.com/hwmm)

[Ad hoc synchronization considered harmful](https://www.usenix.org/events/osdi10/tech/full_papers/Xiong.pdf) 

[Model checking for programming languages using VeriSoft](https://dl.acm.org/doi/abs/10.1145/263699.263717)：第一个 “software model checker”，不记状态；

[Finding and reproducing Heisenbugs in concurrent programs](https://dl.acm.org/doi/10.5555/1855741.1855760)

[Using model checking to find serious file system errors](https://dl.acm.org/doi/10.1145/1189256.1189259)

[VSync: Push-button verification and optimization for synchronization primitives on weak memory models](https://dl.acm.org/doi/abs/10.1145/3445814.3446748)



## 3. 并发编程
