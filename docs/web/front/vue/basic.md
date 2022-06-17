[toc]

# Vue



## props

作为组件的属性key，进行使用。

```vue
<blog-post title="My journey with Vue"></blog-post>

Vue.component('blog-post', {
  props: {
    value: {
      type: String,
      default: ''
    }
  },
  template: '<h3>{{ title }}</h3>'
})
```



## data

**一个组件的 `data` 选项必须是一个函数**，因此每个实例可以维护一份被返回对象的独立的拷贝，作为value使用：

```vue
data() {
    return {
      editor: null
    }
},
```





async

await



./ 表示相对路径，具体代表当前目录下的同级目录，遵从的是从后往前找文件



@/ 的意思：
表示的是相对路径(当然这也是简写啦)，因为这个在根目录/build/webpack.base.conf.js文件中@是配置的，
比如我的配置文件中@就代表src目录，遵从的是从前往后找，比如’@/components/login’ 就表示的是src/components/login文件



## export

export 用来导出模块，Vue 的单文件组件通常需要导出一个对象，这个对象是 Vue 实例的选项对象，以便于在其它地方可以使用 import 引入。

**一个js文件中，只能有一个export default；但是可以多个export**



### vue cli 3.0 显示 vue 源码

https://blog.csdn.net/u010341392/article/details/90209946





keep-alive是Vue提供的一个抽象组件，用来对组件进行缓存，被包裹在keep-alive中的组件的状态将会被保留。