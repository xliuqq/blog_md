[toc]

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

## __attribute__

`__attribute__((constructor))` 与 `__attribute__((destructor)) `是 GCC 中用来修饰函数的

- constructor 可以使被修饰的函数在 **main() 执行前被调用**；
- destructor 可以使被修饰的函数在 **main() 执行结束或 exit() 调用结束后**被执行。
