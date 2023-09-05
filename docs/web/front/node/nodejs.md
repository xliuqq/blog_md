# Nodejs
Node.js® is a JavaScript runtime built on Chrome's V8 JavaScript engine.
NodeJS是JavaScript的运行环境。

## npm修改源
`npm config set registry https://registry.npm.taobao.org`

## npm指定安装路径
```shell
npm config set prefix "D:\AppData\npm\node_modules"
npm config set cache "D:\AppData\npm\node_cache"
```
同时将`$prefix`的值加入到环境变量`PATH`中。



## 依赖组件的版本

- 必须匹配某个版本
  - 如：1.1.2，表示必须依赖1.1.2版

- **\>version**

```
必须大于某个版本
如：>1.1.2，表示必须大于1.1.2版
```

- **~version**

```
大概匹配某个版本
如果minor版本号指定了，那么minor版本号不变，而patch版本号任意
如果minor和patch版本号未指定，那么minor和patch版本号任意
如：~1.1.2，表示>=1.1.2<1.2.0，可以是1.1.2，1.1.3，1.1.4，…，1.1.n
如：~1.1，表示>=1.1.0 <1.2.0，可以是同上
如：~1，表示>=1.0.0<2.0.0，可以是1.0.0，1.0.1，1.0.2，…，1.0.n，1.1.n，1.2.n，…，1.n.n
```

- **^version**

```
兼容某个版本
版本号中最左边的非0数字的右侧可以任意
如果缺少某个版本号，则这个版本号的位置可以任意
如：^1.1.2 ，表示>=1.1.2
<2.0.0，可以是1.1.2，1.1.3，…，1.1.n，1.2.n，…，1.n.n
如：^0.2.3 ，表示>=0.2.3 <0.3.0，可以是0.2.3，0.2.4，…，0.2.n
如：^0.0，表示 >=0.0.0 <0.1.0，可以是0.0.0，0.0.1，…，0.0.n
```
