[toc]

# GitBook

使用markdown编写书籍。https://github.com/GitbookIO/gitbook/issues

GitBook的代码在2019年停止更新，可以使用[mdbook](https://mdbook.budshome.com/cli/serve.html)进行替代

## 安装

```shell
# 安装nodejs，nodejs.org
# 查看版本检查是否安装成功
$ sudo node -v
$ sudo npm -v
# 通过npm安装GitBook
$ npm install gitbook-cli -g
# 查看GitBook版本同时安装
$ gitbook -V
# 新建文件夹来作为一本书
$ mkdir mybook
# 进入该目录，初始化gitbook
$ cd mybook
$ gitbook init
# 编辑目录下的SUMMARY.md文件，形成章节结构，如下
#	* [序言](README.md)
#	* [第1章](chapter1/README.md)
#	 * [第1节](chapter1/c1s1.md)
#	 * [第2节](chapter1/c1s2.md)
#	* [第2章](chapter2/README.md)
#	 * [第1节](chapter2/c1s1.md)
#	 * [第2节](chapter2/c1s2.md)
#	* [结束](end/README.md)
# 运行下面命令来构建章节结构
$ gitbook build
# 或者直接运行下面命令可在浏览器查看书本信息
$ gitbook serve
# 浏览器中输入 http://localhost:4000 ，即可查看
```

## 结构

在gitbook文件夹中打开git bash命令窗口执行gitbook init命令初始化书籍目录生成README.md和SUMMARY.md两个文件

**README.md**

- REAME相当于书籍的前言部分, 可以忽略

**SUMMARY.md**

- SUMMARY是最重要的一个部分, 它创建的是整书的索引，定义了你的图书的结构



## 插件

流程图

`npm install gitbook-plugin-mermaid`

