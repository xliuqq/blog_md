## Vue Loader

Vue Loader 是一个 [webpack](https://webpack.js.org/) 的 loader，它允许你以一种名为[单文件组件 (SFCs)](https://vue-loader.vuejs.org/zh/spec.html)的格式撰写 Vue 组件：

```vue
<template>
  <div class="example">{{ msg }}</div>
</template>

<script>
export default {
  data () {
    return {
      msg: 'Hello world!'
    }
  }
}
</script>

<style>
.example {
  color: red;
}
</style>
```

Vue Loader 还提供了很多酷炫的特性：

- 允许为 Vue 组件的每个部分使用其它的 webpack loader，例如在`<style>`的部分使用 Sass 和在 `<template>`的部分使用 Pug；
- 允许在一个 `.vue` 文件中使用自定义块，并对其运用自定义的 loader 链；
- 使用 webpack loader 将 `<style>`和 `<template>` 中引用的资源当作模块依赖来处理；
- 为每个组件模拟出 scoped CSS；
- 在开发过程中使用热重载来保持状态。

