[toc]

# Python



## 解释器

### CPython

**官方**版本的解释器：Cpython，用C语言开发的，运行python，就是启动CPython解释器，CPython是使用最广的Python解释器

### IPython

IPython是基于CPython之上的一个**交互式**解释器

### PyPy

PyPy是用Python实现的解释器，PyPy采用JIT技术，对Python代码进行动态编译

### Jython

Jython是运行在Java平台上的Python解释器，可以直接把Python代码编译成Java字节码执行



## 基础

**转为字符串方法**

- str() :  字符串的形式
- repr() : 作用在字符串上时会多一层引号



## 子进程(TODO)

os.popen



## -m

python -m run.py

作用是：把 run.py 文件当做模块启动

**直接启动是把run.py文件，所在的目录放到了sys.path属性中。模块启动是把你输入命令的目录（也就是当前路径），放到了sys.path属性**



## Psutil

在Python中获取系统信息的模块，包括：

- CPU信息，逻辑核数、物理核数，cpu_times（用户/系统/空闲时间）；
- top；
- 物理内存和交换区信息；
- 磁盘信息，磁盘分区、使用率和IO；
- 网络接口和网络连接信息；
- 进程的详细信息；

