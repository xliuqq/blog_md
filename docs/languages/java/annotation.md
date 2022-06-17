# 注解

## 元注解

四种元注解，java.lang.annotation，专门注释其他的注解：

- @Documented：注解是否将包含在JavaDoc中

- @Retention：什么时候使用该注解，注解的生命周期，RetentionPolicy

  - SOURCE：编译阶段丢弃，注解在编译结束无意义，如@Override,       @suppressWarnings；
  - CLASS：类加载时候丢弃，在字节码文件的处理中有用，默认方式；
  - RUNTIME：不丢弃，运行期也保留注解，可以使用反射机制读取注解信息；自定义注解使用；

- @Target：注解用于什么地方，ElementType.{TYPE, METHOD,…}
- @Inherited：是否允许子类继承该注解

> 注解定义时，不可以继承其它的注解或接口；

@interface用来声明一个注解，其中的每一个方法实际上是声明了一个配置参数。方法的名称就是参数的名称，返回值类型就是参数的类型（返回值类型只能是基本类型、Class、String、enum）。可以通过default来声明参数的默认值。

```java
@Documented
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
public @interface Action {
    String input() default "";
    FaultAction[] fault() default { };
}
```



### 常用的注解

#### @GuardedBy

@GuardedBy("mResourcesManager")

是一个同步锁，只有在**线程持有mResourcesManager对象锁**时，才能使用这写变量。



## 处理器

### Processor

`javax.annotation.processing.Processor`

注解处理器(Annotation Processor)是javac内置的一个用于编译时扫描和处理注解(Annotation)的工具，常用的用法就是在编译期间获取相关注解数据，然后动态生成.java源文件，比如lombok。