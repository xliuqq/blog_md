[toc]

# Vue CLI

Vue CLI 是一个基于 Vue.js 进行快速开发的完整系统。

- 通过 `@vue/cli` 搭建交互式的项目脚手架。

- 通过 `@vue/cli` + `@vue/cli-service-global` 快速开始零配置原型开发。

- 一个运行时依赖 (

  ```
  @vue/cli-service
  ```

  )，该依赖：

  - 可升级；
  - **基于 webpack 构建，并带有合理的默认配置**；
  - 可以通过项目内的配置文件进行配置；
  - 可以通过插件进行扩展。

- 一个丰富的官方插件集合，集成了前端生态中最好的工具。

- 一套完全图形化的创建和管理 Vue.js 项目的用户界面。

Vue CLI 致力于将 Vue 生态中的工具基础标准化。



## 组件

### CLI

CLI (`@vue/cli`) 是一个全局安装的 npm 包，提供了终端里的 `vue` 命令。

```bash
npm install -g @vue/cli
# OR
yarn global add @vue/cli
```

#### vue serve

 `vue serve` 和 `vue build` 命令对单个 `*.vue` 文件进行**快速原型开发**，但是需要先安装全局扩展：

```bash
npm install -g @vue/cli-service-global
```

`vue serve` 的缺点就是它需要安装全局依赖，这使得它在**不同机器上的一致性不能得到保证**。因此这只适用于快速原型开发。


```text
Usage: serve [options] [entry]
在开发环境模式下零配置为 .js 或 .vue 文件启动一个服务器
Options:
  -o, --open  打开浏览器
  -c, --copy  将本地 URL 复制到剪切板
  -h, --help  输出用法信息
```

`vue serve` 使用了和 `vue create` 创建的项目相同的默认设置 (webpack、Babel、PostCSS 和 ESLint)。它会在当前目录自动推导入口文件——入口可以是 `main.js`、`index.js`、`App.vue` 或 `app.vue` 中的一个。也可以显式地指定入口文件：

```bash
vue serve MyComponent.vue
```

#### vue build

```text
Usage: build [options] [entry]
在生产环境模式下零配置构建一个 .js 或 .vue 文件
Options:
  -t, --target <target>  构建目标 (app | lib | wc | wc-async, 默认值：app)
  -n, --name <name>      库的名字或 Web Components 组件的名字 (默认值：入口文件名)
  -d, --dest <dir>       输出目录 (默认值：dist)
  -h, --help             输出用法信息
```

#### vue create

```bash
vue create hello-world
```

通过命令行创建项目

#### vue ui

通过UI界面创建项目

### CLI 服务

CLI 服务 (`@vue/cli-service`) 是一个开发环境依赖。在一个 Vue CLI 项目中，`@vue/cli-service` 安装了一个名为 `vue-cli-service` 的命令。

CLI 服务是构建于 [webpack](http://webpack.js.org/) 和 [webpack-dev-server](https://github.com/webpack/webpack-dev-server) 之上的。它包含了：

- 加载其它 CLI 插件的核心服务；
- 一个针对绝大部分应用优化过的内部的 webpack 配置；
- 项目内部的 `vue-cli-service` 命令，提供 `serve`、`build` 和 `inspect` 命令。

`@vue/cli-service` 实际上大致等价于 `react-scripts`，尽管功能集合不一样。

#### vue-cli-service serve

```text
用法：vue-cli-service serve [options] [entry]
选项：
  --open    在服务器启动时打开浏览器
  --copy    在服务器启动时将 URL 复制到剪切版
  --mode    指定环境模式 (默认值：development)
  --host    指定 host (默认值：0.0.0.0)
  --port    指定 port (默认值：8080)
  --https   使用 https (默认值：false)
```

`vue-cli-service serve` 命令会启动一个开发服务器 (基于 [webpack-dev-server](https://github.com/webpack/webpack-dev-server)) 并附带开箱即用的模块热重载 (Hot-Module-Replacement)。

#### vue-cli-service build

```text
用法：vue-cli-service build [options] [entry|pattern]
选项：
  --mode        指定环境模式 (默认值：production)
  --dest        指定输出目录 (默认值：dist)
  --modern      面向现代浏览器带自动回退地构建应用
  --target      app | lib | wc | wc-async (默认值：app)
  --name        库或 Web Components 模式下的名字 (默认值：package.json 中的 "name" 字段或入口文件名)
  --no-clean    在构建项目之前不清除目标目录
  --report      生成 report.html 以帮助分析包内容
  --report-json 生成 report.json 以帮助分析包内容
  --watch       监听文件变化
```

`vue-cli-service build` 会在 `dist/` 目录产生一个可用于生产环境的包，带有 JS/CSS/HTML 的压缩，和为更好的缓存而做的自动的 vendor chunk splitting。它的 chunk manifest 会内联在 HTML 里。

### CLI 插件

CLI 插件是向你的 Vue 项目提供可选功能的 npm 包，例如 Babel/TypeScript 转译、ESLint 集成、单元测试和 end-to-end 测试等。Vue CLI 插件的名字以 `@vue/cli-plugin-` (内建插件) 或 `vue-cli-plugin-` (社区插件) 开头，非常容易使用。

当你在项目内部运行 `vue-cli-service` 命令时，它会自动解析并加载 `package.json` 中列出的所有 CLI 插件。

插件可以作为项目创建过程的一部分，或在后期加入到项目中。它们也可以被归成一组可复用的 preset。我们会在[插件和 preset](https://cli.vuejs.org/zh/guide/plugins-and-presets.html) 章节进行深入讨论



## 配置

默认情况下，一个 Vue CLI 项目有三个模式：

- `development` 模式用于 `vue-cli-service serve`
- `production` 模式用于 `vue-cli-service build` 和 `vue-cli-service test:e2e`
- `test` 模式用于 `vue-cli-service test:unit`

每个模式都会将 `NODE_ENV` 的值设置为模式的名称——比如在 development 模式下 `NODE_ENV` 的值会被设置为 `"development"`



### WebStorm 配置自定义路径@

`Vue CLI 3` 里，是没有传统的 webpack 配置文件的，因为使用另一种写法集成到了 `vue.config.js` 里。

```js
/**
 * 由于 Vue CLI 3 不再使用传统的 webpack 配置文件，故 WebStorm 无法识别别名
 * 本文件对项目无任何作用，仅作为 WebStorm 识别别名用
 * 进入 WebStorm preferences -> Language & Framework -> JavaScript -> Webpack，选择这个文件即可
 * */
const resolve = dir => require('path').join(__dirname, dir);
 
module.exports = {
    resolve: {
        alias: {
            '@': resolve('src')
        }
    }
};
```

