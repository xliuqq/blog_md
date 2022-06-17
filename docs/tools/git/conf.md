# Git 配置

git status显示修改了大量文件，git diff 提示 **filemode变化**

```shell
diff --git a/Android.mk b/Android.mk
old mode 100644
new mode 10075
```

原来是filemode的变化，文件chmod后其文件某些位是改变了的，如果严格的比较原文件和chmod后的文件，两者是有区别的，但是源代码通常只关心文本内容，因此chmod产生的变化应该忽略，所以设置一下：

切到源码的根目录下：

```shell
git config --add core.filemode false
```



### core.autocrlf

- git config --global core.autocrlf true

  提交时自动地把行结束符CRLF转换成LF，而在签出代码时把LF转换成CRLF。

- git config --global core.autocrlf input

  Git在提交时把CRLF转换成LF，签出时不转换

- git config --global core.autocrlf false

  都不进行转换





#### GIT 拉取或者提交项目时， 遇到长路径提示file name too long

git config --system core.longpaths true