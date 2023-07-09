# 类加载

**类加载器：通过一个类的全限定名来获取描述此类的二进制字节流**。

类由类加载器和类本身确定其在虚拟机中的唯一性；

- 类“相等”（Class对象的equals, isAssignableFrom, isInstance方法），必须由**同一个加载器**加载前提下才有意义；加载器不同，则肯定不同；





## 双亲委派模型

> - 引导类加载器：定义核心Java SE和JDK模块。
> - 平台类加载器：定义部分Java SE和JDK模块。
> - 应用或系统类加载器：定义CLASSPATH上的类和模块路径中的模块

将加载交给父加载器加载，父加载器无法加载，则自己再加载；（类和类加载器有了优先层级）

- 组合而不是继承实现；



### 引导类加载器

C++语言实现，是虚拟机自身一部分；

- 将`<JAVA_HOME>\lib`目录中或`-Xbootclasspath`指定路径，仅被虚拟机识别的类库加载到虚拟机。
- 不可以被Java程序直接引用，如`String.class.getClassLoader()` 返回的结果为 null；



### 平台(扩展)类加载器

> JDK 9之后称为 Platform ClassLoader。

`<JAVA_HOME>\lib\ext`目录或`java.ext.dirs`指定路径，可以被Java程序使用；



### 系统类加载器

> AppClassLoader

应用程序类加载器：用户类路径（ClassPath）上类库，可以直接被Java使用；



### 线程加载器

> 双亲委派模型的典型问题是加载 SPI 实现类的场景。

Java中的 SPI 机制，如 JDBC：

- Driver类由启动类加载器加载（rt.jar），但是实现是由第三方提供的，启动类加载器加载不到；

- 第三方提供的具体 jar 实现，`ServiceLoader`通过 由Thread的`contextClassLoader`加载；

- 线程上下文类加载器，继承于父线程，若开始就没有，则**默认是系统类加载器**（AppClassLoader）；



## JDK 9

模块化的支持，对双亲委派模式做了一些改动

- 扩展类加载器被平台类加载器（Platform ClassLoader）
  - 模块化，可扩展，无需`java.ext.dirs`

- 启动类加载器现在是在 Java 虚拟机内部和 Java 类库共同协作实现的类加载器（以前是 C++实现）
  - 即存在`BootClassLoader`类，但所有在获取启动类加载器的场景仍然会返回 null 来代替（兼容）；

- 平台类加载器和系统类加载器不再是`URLClassLoader`类的对象，而继承`BuiltinClassLoader`：
  - Hack失效：把系统类加载器转型成`URLClassLoader`并调用其`addURL()`方法

- 当平台及应用程序类加载器收到类加载请求，在委派给父加载器加载前，要先**判断该类是否能够归属到某一个系统模块**中，如果可以找到这样的归属关系，就要**优先委派给负责那个模块的加载器**完成加载。

![img](pics/类加载-双亲委派模式.jpeg)

## 自定义类加载器

- 覆盖findClass方法；

