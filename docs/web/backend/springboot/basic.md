# 基础

## 注解

### 常用注解

**@Controller**，**@RestController**

**@GetMapping**，**@PostMapping**

**@RequestBody**，**@ResponseBody**

**@ControllerAdvice**

- 和`@ExceptionHandler`、`@InitBinder`以及`@ModelAttribute`注解搭配使用，主要是用来处理控制器所抛出的异常信息；

**@ExceptionHandler**

**@RequestParam**，**@PathVariable**

**@CrossOrigin**



**@Component**，**@ComponentScan**



Spring DI注解：

**@DependsOn**

**@Bean**

**@Scope**

- singleton、prototype、request、session、globalSession



**@Autowired**

**@Primary**

- 定义这些Bean的优先级



### SpringBoot注解

**@SpringBootApplication**

**@EnableAutoConfiguration**

**@ConditionalOnClass与@ConditionalOnMissingClass**

**@ConditionalOnMissingBean**，**@ConditionalOnMissingBean**

**@ConditionalOnProperty**，**@ConditionalOnResource**



### 第三方包的注解生效

#### @ComponentScan

@SpringBootApplication注解默认扫描只是当前项目的**本包**和**它的子包**，**@ComponentScan**主要就是定义**扫描的路径**从中找出标识了**需要装配的类**自动装配到spring的bean容器中。

#### @Import

@ComponentScan是扫描整个包,但其实你可能只需注入一个或者几个指定的Bean，那我们可以考虑用 **@Import** 注解。

#### spring.factories

上面两种注入方式都有个很明显缺点，就是如果我需要引用外部jar包的Bean的时候，都需要在当前项目配置 **@ComponentScan** 或者 **@Import** 去扫描才能注入当前Bean，这样显然不够友好。

只需要在将**配置放在第三方jar指定的文件**中即可，使用者会自动加载，从而**避免的代码的侵入**

- 在资源目录下新建目录 META-INF
- 在 META-INF 目录下新建文件 **spring.factories**
- 在文件中添加下面配置

```stylus
org.springframework.boot.autoconfigure.EnableAutoConfiguration=com.third.bean.ConfigurationBean
```



## Spring 全局资源的加载

实现CommandLIneRunner 或者 ApplicationRunner，实现 run 方法

### Runner

项目启动后立马执行一些逻辑：

```java
@SpringBootApplication(exclude = DataSourceAutoConfiguration.class)
public class Agent {
    public static void main(String[] args) {
        SpringApplication.run(Agent.class, args);
        // Want to execute other logic
    }
}
```

如果**采用tomcat服务，则运行的时候，run后面的逻辑不会执行**。

**解决方法**：

**CommandLineRunner** 是 Spring Boot 提供的一个接口，当你实现该接口并将之注入Spring IoC容器后，Spring Boot应用启动后就会执行其run方法。

- **ApplicationRunner** 接收一个封装好的对象参数ApplicationArguments。除此之外它们功能完全一样

```java
@Slf4j
@Component
public class HighOrderCommandLineRunner implements CommandLineRunner, Ordered {
    @Override
    public void run(String... args) throws Exception {
        // 不应该阻塞，如果需要阻塞，则另起线程
        for (String arg : args) {
            log.info("arg = " + arg);
        }
        log.info("i am highOrderRunner");
    }

    @Override
    public int getOrder() {
        return Integer.MIN_VALUE+1;
    }
}
```

