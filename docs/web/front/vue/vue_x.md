# Vue-X



## 本地存储

vuex存储在内存，localstorage（本地存储）则以文件的方式存储在本地，永久保存；sessionstorage( 会话存储 ) ，临时保存。

当刷新页面（这里的刷新页面指的是 --> F5刷新，属于清除内存了）时vuex存储的值会丢失，sessionstorage页面关闭后就清除掉了，localstorage不会。 

### sessionStorage 

sessionStorage 方法针对一个 session 进行数据存储。当用户关闭浏览器窗口后，数据会被删除。

### localStorage 

localStorage 方法存储的数据没有时间限制。第二天、第二周或下一年之后，数据依然可用。



## 使用

简单的 Vue 计数应用

```js
new Vue({
  // state
  data () {
    return {
      count: 0
    }
  },
  // view
  template: `
    <div>{{ count }}</div>
  `,
  // actions
  methods: {
    increment () {
      this.count++
    }
  }
})
```

vuex的用法

```js
const store = new Vuex.Store({
  state: {
    count: 0
  },
  mutations: {
    increment (state) {
      state.count++
    }
  }
})
```

使用

```js
store.commit('increment')

console.log(store.state.count) // -> 1
```

通过提交 mutation 的方式，而非直接改变 `store.state.count`，是因为我们想要更明确地追踪到状态的变化。



## 概念

### State

### Getter

多个组件需要用到此属性，在 store 中定义“getter”（可以认为是 store 的计算属性）。就像计算属性一样，**getter 的返回值会根据它的依赖被缓存起来**，且只有当它的依赖值发生了改变才会被重新计算。

```js
const store = new Vuex.Store({
  state: {
    todos: [
      { id: 1, text: '...', done: true },
      { id: 2, text: '...', done: false }
    ]
  },
  getters: {
    doneTodos: state => {
      return state.todos.filter(todo => todo.done)
    },
    // Getter 也可以接受其他 getter 作为第二个参数。
    doneTodosCount: (state, getters) => {
      return getters.doneTodos.length
    }
  }
})
```

Getter 会暴露为 `store.getters` 对象，你可以以属性的形式访问这些值：

```js
store.getters.doneTodos // -> [{ id: 1, text: '...', done: true }]
```

让 **getter 返回一个函数，来实现给 getter 传参**。在你对 store 里的数组进行查询时非常有用。

```js
getters: {
  // ...
  getTodoById: (state) => (id) => {
    return state.todos.find(todo => todo.id === id)
  }
}

store.getters.getTodoById(2) // -> { id: 2, text: '...', done: false }
```

**getter 在通过方法访问时，每次都会去进行调用，而不会缓存结果**。

### Mutation

Vuex 中的 mutation 非常类似于事件：每个 mutation 都有一个字符串的 **事件类型 (type)** 和 一个 **回调函数 (handler)**。以相应的 type 调用 **store.commit** 方法：

```js
store.commit('increment')
```

dispatch：含有异步操作，例如向后台提交数据，写法： this.$store.dispatch('mutations方法名',值)

commit：同步操作，写法：this.$store.commit('mutations方法名',值)

```vue
this.$store.commit('toShowLoginDialog', true);
this.$store.dispatch('toShowLoginDialog',false);
```



### Action

Action 类似于 mutation，不同在于：

- Action 提交的是 **mutation，而不是直接变更状态**。
- Action 可以包含**任意异步操作**。

```js
const store = new Vuex.Store({
  state: {
    count: 0
  },
  mutations: {
    increment (state) {
      state.count++
    }
  },
  actions: {
    increment (context) {
      context.commit('increment')
    }
  }
})
```

Action 函数接受一个与 store 实例具有相同方法和属性的 context 对象，因此你可以调用 `context.commit` 提交一个 mutation，或者通过 `context.state` 和 `context.getters` 来获取 state 和 getters。

### Module

由于使用单一状态树，应用的所有状态会集中到一个比较大的对象。当应用变得非常复杂时，store 对象就有可能变得相当臃肿。

为了解决以上问题，Vuex 允许我们将 store 分割成**模块（module）**。每个模块拥有自己的 state、mutation、action、getter、甚至是嵌套子模块——从上至下进行同样方式的分割

```js
const moduleA = {
  state: { ... },
  mutations: { ... },
  actions: { ... },
  getters: { ... }
}

const moduleB = {
  state: { ... },
  mutations: { ... },
  actions: { ... }
}

const store = new Vuex.Store({
  modules: {
    a: moduleA,
    b: moduleB
  }
})

store.state.a // -> moduleA 的状态
store.state.b // -> moduleB 的状态
```

对于模块内部的 action，局部状态通过 `context.state` 暴露出来，根节点状态则为 `context.rootState`

```js
const moduleA = {
  // ...
  actions: {
    incrementIfOddOnRootSum ({ state, commit, rootState }) {
      if ((state.count + rootState.count) % 2 === 1) {
        commit('increment')
      }
    }
  }
}
```



### 命名空间

#### 在带命名空间的模块内访问全局内容（Global Assets）

#### 在带命名空间的模块注册全局 action


