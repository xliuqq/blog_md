[toc]

# Spring AOP

## IoC：Inverse of Control

- 通过**接口抽象和实现**，在**配置文件中定义具体实现类，通过反射机制构造实例**；
- IOC的实现方式有两种：依赖查找(DL)和依赖注入(DI)；



## Spring拦截对比

| 拦截方式               | 特点                                                         |
| ---------------------- | ------------------------------------------------------------ |
| servlet的过滤器Filter  | 可以拿到原始的http请求和响应的信息，但是拿不到真正处理这个请求的方法的信息。在Filter中是不知道这个请求是哪个控制器的哪个方法来处理的。如果你需要这个信息的话，那么需要使用springmvc的interceptor。 |
| springmvc的interceptor | 拦截器可以拿到原始的http请求和响应的信息，也能拿到真正处理这个请求的方法的信息，但是其**拿不到这个方法被调用的时候真正调用的参数的值** |
| spring的切片Aspect     | 可以拿到方法被调用的时候真正调用的参数的值，但是拿不到原始的http请求和响应的信息 |



## AOP：Aspect Oriented Programming 

- OOP关注纵向，AOP关注横向，如日志、安全型、异常处理，散布在所有对象层次中；

  - 横切关注点：对哪些方法进行拦截，拦截后怎么处理？
  - **切面(aspect)**：对横切关注点的抽象；
  - **连接点(joinpoint)**：被拦截到的点，Spring只支持方法类型的连接点，实际连接点还可以是字段或构造器；
  - **切入点(pointcut)**：对连接点进行拦截的定义；
  - 通知(advice)：指拦截到连接点之后要执行的代码，通知分为前置、后置、异常、返回、环绕通知五类；
  - 引入(introcution)：在不改代码前提，引入可以在运行期为类动态地添加方法和字段；
  - 织入(weaveing)：Spring使用动态代理织入，AspectJ采用编译器织入和类装载器织入；

- Pointcut可以有args()，execution()，this()，within()，并可以通过&& || ! 方式组合

  - execution：匹配满足条件的方法称为连接点
  - this(a)：实现接口a的所有类，若a不是接口，则限定a单个类
  - within(a..*)：满足a的包以及子包的任意类
  - args()：可以指定方法的参数名，可以传入到advice中；
  - target()：不同于this，this指向真正对象，targe执行代理对象，因为Spring      AOP是基于代理的，可选JDK      proxy或CGLIB；

- `execution(modifiers-pattern? ret-type-pattern declaring-type-pattern? name-pattern(param-pattern) throws-pattern?)`：

  - 其中，除了返回值类型ret-type-pattern，方法名name-pattern，参数param-pattern，其余是可以省略的；
  - *表示所有（任何都匹配成功），如可以表示任何返回类型，任何方法名，任何参数类型；

  - 对于方法名，abc.*表示包abc下的所有方法，abc..*表示包abc以及其子包下的方法；
  - 对于参数，()表示匹配无任何参数的方法，(*)表示匹配任何类型的单参数方法，(..)匹配所有方法；

- **advice**

  - before：连接点执行前
  - around：制定方法执行，等价于before      + 执行 +      after
  - after：连接点执行后，finally特性
  - after-returning：连接点正常返回时，returning制定返回值的名称，与advice的参数名一致；
  - after-throwing：连接点抛出指
  - 定异常时，执行advice

### 静态方法切点

`org.springframework.aop.support.StaticMethodMatcherPointcut`

`StaticMethodMatcherPointcut`是静态方法切点的抽象基类，默认情况下匹配所有的类。`StaticMethodMatcherPointcut`有两个重要的子类：`NameMethodMatcherPointcut`和`AbstractRegexMethodPoint`。前者提供简单的字符串匹配方法签名，后者使用正则表达式匹配方法签名。





## SpringBoot AOP

```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-aop</artifactId>
</dependency>
```

Java代码：

```java
@Aspect
   
@Pointcut("execution(public * com.stuPayment.controller..*.*(..))")//切入点描述 这个是controller包的切入点

@Before("controllerLog() || uiControllerLog()")
```

