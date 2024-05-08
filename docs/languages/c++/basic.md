# C++



## \# 和 \##

\#  -- 转换， 完成代码到字符串的转换

```c++
#define CONVERT(name) #name
 
int main(int argc, char* argv[])
{
    printf("You and %s are friends.\n", CONVERT(James));
    return 0;
}
```

\## -- 连接， 完成代码的连接

 ```c++
#define CAT(batman, robin) batman ## robin
 
#define make_friend(index)  printf("You and %s are friends.\n", CAT(james, index));
 
int main(int argc, char* argv[])
{
    char* james001="fake James 001";
    char* james007="James Bond";
    char* james110="fake James 110";
 
    make_friend(001); // print the james001 variable
    make_friend(007);
    make_friend(110);
    return 0;
}
 ```

## \__attribute__

`__attribute__((constructor))` 与 `__attribute__((destructor)) `是 GCC 中用来修饰函数的

- constructor 可以使被修饰的函数在 **main() 执行前被调用**；
- destructor 可以使被修饰的函数在 **main() 执行结束或 exit() 调用结束后**被执行。



## Buffer

libc will **line-buffer when stdout to screen** and **block-buffer when stdout to a file**, but no-buffer for stderr.



## extern C

> C++ 的关键字，用于引入 C 的头文件

- 被 extern "C" 限定的函数或变量是<font color='red'>**`extern` 类型**</font>的
  - 表明函数和全局变量作用范围（可见性）的关键字，可以被外部模块使用
- 被 extern "C" 修饰的变量和函数是<font color='red'>**按照 `C` 语言方式编译和连接**</font>的。
  - C中的函数`C++` 编译后在符号库中的名字与 `C` 语言的有所不同（因为C++支持函数重载）

示例：C 中的库，如<stdlib.h>，都在其定义的时候，使用了`extern C`

- 如果 C++ 中使用 C 的库，如 stdlib，应该引入 \<cstdlib> ，因为会将函数限定在 `std` 的名空间；

```c
#ifndef __BEGIN_DECLS
# ifdef  __cplusplus
#  define __BEGIN_DECLS  extern "C" {
#  define __END_DECLS    }
# else
#  define __BEGIN_DECLS
#  define __END_DECLS
# endif
#endif
```

## ABI

 针对 library （主要是 shared library，即动态链接库）的 ABI (application binary interface)，当 library 升级时，依赖该库的二进制不需要改动。

- non-virtual 函数比 virtual 函数更健壮：因为 **virtual function 是 bind-by-vtable-offset，而 non-virtual function 是 bind-by-name**。



### 二进制代码不兼容例子

- 给函数增加**默认参数**，现有的可执行文件无法传这个额外的参数；
- 增加**虚函数**，会造成 vtbl 里的排列变化（不要考虑“只在末尾增加”这种取巧行为，因为你的 class 可能已被继承。）；
- 增加**默认模板类型参数**，比方说 `Foo<T>` 改为 `Foo<T, Alloc=alloc<T>>`，这会改变 name mangling；



给 class Bar 增加数据成员，造成 sizeof(Bar) 变大，以及内部数据成员的 offset 变化：

- 不安全（不兼容）的客户端使用：

  - 客户代码里有 `new Bar`，那么肯定不安全；

  - 客户代码里有 `Bar* pBar; pBar->memberA = xx`，那么肯定不安全，memberA 的新 Bar 的偏移可能会变；

  - 如果客户调用 `pBar->setMemberA(xx);` 而 `Bar::setMemberA()` 是个 inline function，那么肯定不安全，因为偏移量已经被 inline 到客户的二进制里。

- 兼容的客户端使用：
  - 通过成员函数访问对象的数据成员，且该成员函数定义在 cpp 中，不是内联函数；



### 解决办法

第一种类似**桥接**设计模式：间接调用实现类实现，可能有一定的性能损失；

- 接口类：只定义 non-virtual 接口，只包含 `Impl *` 的私有成员；
- 接口类新增函数，只要新增 non-virtual 接口；



第二种：

- 对于接口类 Bar：

  - 所有的成员都是私有成员，并通过非内联的成员函数调用；
  - 修改时，只新增 non-virtual 接口；

  - **不提供公共的构造函数和析构函数**，而是提供工厂方法返回`Bar *`；

- 对于客户端要求：
  - 客户端不需要用到 `sizeof(Bar)`或者接口类不要添加新成员；
