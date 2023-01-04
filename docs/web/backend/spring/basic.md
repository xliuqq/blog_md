# 基础

## Bean

### 注册

#### @Configuration + @Bean

```java
@Configuration
public class MyConfiguration {
    @Bean
    public Person person() {
        Person person = new Person();
        person.setName("spring");
        return person;
    }
}
```

#### @Componet + @ComponentScan

```java
@Component
@Data
public class Person {
    private String name;
}
 
@ComponentScan(basePackages = "com.springboot.initbean.*")
public class Demo1 {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(Demo1.class);
        Person bean = applicationContext.getBean(Person.class);
        System.out.println(bean);
    }
}
```

#### @Import注解导入

直接使用

```java
// Person类上不需要@Component注解
@Data
public class Person {
    private String name;
}
/**
* 直接使用@Import导入person类，然后尝试从applicationContext中取，成功拿到
**/
@Import(Person.class)
public class Demo1 {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(Demo1.class);
        Person bean = applicationContext.getBean(Person.class);
        System.out.println(bean);
    }
}
```

@ImportSelector定义加载的类

```java
@Import(MyImportSelector.class)
public class Demo1 {
 
    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext(Demo1.class);
        Person bean = applicationContext.getBean(Person.class);
        System.out.println(bean);
    }
}
class MyImportSelector implements ImportSelector {
    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        return new String[]{"com.springboot.pojo.Person"};
    }
}
```



### 资源清理

退出容器时执行操作，通过

- 继承DisposableBean实现，或
- 注解@PreDestory实现



## 校验

### @Valid 和 @Validated 比较

- @Valid 和 @Validated 两者都可以对数据进行校验，待校验字段上打的规则注解（@NotNull, @NotEmpty等）都可以对 @Valid 和 @Validated 生效；
- @Valid 进行校验的时候，需要用 BindingResult 来做一个校验结果接收。当校验不通过的时候，如果手动不 return ，则并不会阻止程序的执行；
- @Validated 当校验不通过的时候，程序会抛出BindException，阻止方法中的代码执行，需要再写一个全局校验异常捕获处理类，然后返回校验提示。

```java

@RestControllerAdvice
@ResponseBody
public class ValidExceptionHandler {
 
    @ExceptionHandler(BindException.class)
    public  Result<String> validExceptionHandler(BindException exception) {
        return Result.fail(HttpStatus.BAD_REQUEST.value(), e.getAllErrors().get(0).getDefaultMessage());
    }
}
```

