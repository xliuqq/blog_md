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

