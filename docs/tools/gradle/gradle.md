# Gradle

### Configuration 概念

**Gradle** 将对依赖进行分组，比如编译 *Java* 代码时使用一组，运行 *Java* 代码时有使用另一组依赖。每一组依赖我们称为一个 **Configuration**，在声明依赖时，实际上是在设置不同的 **Configuration**。定义一个 **Configuration**：

```undefined
configurations {
   myDependency
}
```

通过 `dependencies()` 方法向 `myDependency` 中加入实际的依赖项：

```bash
dependencies {
   myDependency 'org.apache.commons:commons-lang3:3.0'
}
```

 **Task** 需要使用 *Apache Commons*，比如需要将其加入到 *classpath* 中

```go
task showMyDependency {
    doLast {
        println configurations.myDependency.asPath
    }
}
```

**依赖项范围**

Gradle的依赖项的scope：

- **compileOnly**：仅编译时依赖项，在运行时不使用；不会暴露在类库使用者的编译时（编译时不会出现gson的依赖，即传递依赖）。
- **implementation**：依赖在编译和运行时均包含
- **runtimeOnly**：仅运行时依赖项
- **testImplementation**：仅实现测试的依赖项
- **testRuntimeOnly**：仅用于编译测试，在运行时不使用

```
dependencies {
    implementation 'org.hibernate:hibernate-core:3.6.7.Final'
    testImplementation  ‘junit:junit:4.+'
}
```



**动态管理**

- **支持动态的版本依赖**：在版本号后面使用+号的方式可以实现动态的版本管理。



**多模块**

- parent的`build.gradle`中可以使用`allprojects`和`subprojects`代码块来分别定义里面的配置是应用于所有项目还是子项目；
- 子模块的定义是放置在setttings.gradle文件中；



**插件**

```
task dropDB(type: Exec) {
 commandLine ‘curl’,’-s’,’s’,’-x’,’DELETE’,"http://${db.server}:{db.port}/db_name"
}
```

