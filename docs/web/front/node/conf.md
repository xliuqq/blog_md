#### **devDependencies和dependencies的区别**

devDependencies里面的插件只用于开发环境，不用于生产环境，即辅助作用，打包的时候需要，打包完成就不需要了。而dependencies是需要发布到生产环境的，自始至终都在。比如wepack等只是在开发中使用的包就写入到devDependencies，而像vue这种项目全程依赖的包要写入到devDependencies。

