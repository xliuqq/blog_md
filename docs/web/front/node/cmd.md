# NPM CMD

## node

node 命令可以直接执行 js 文件。

## npm run

npm run是npm run-script的简写，执行的脚本配置在 **package.json** 中的 **scripts** 对象。

```json
// package.json
{
	"scripts": {
		"sayHello": "echo hello world"
	}
}
```

`npm run sasyHello` 会执行这个命令。

除了系统的$PATH，`npm run`会将`node_modules/.bin`加到PATH变量中。

## npm install

npm install module_name **-S**  即  npm install module_name --save  写入dependencies

npm install module_name **-D**  即  npm install module_name --save-dev 写入devDependencies



## registry

```bash
npm --registry https://registry.npm.taobao.org install express
```



```bash
npm config set registry https://registry.npm.taobao.org
```

