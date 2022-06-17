# Nodejs
Node.js® is a JavaScript runtime built on Chrome's V8 JavaScript engine.
NodeJS是JavaScript的运行环境。

## npm修改源
npm config set registry https://registry.npm.taobao.org

## npm指定安装路径
```shell
npm config set prefix "D:\AppData\npm\node_modules"
npm config set cache "D:\AppData\npm\node_cache"
```
同时将`$prefix`的值加入到环境变量`PATH`中。

