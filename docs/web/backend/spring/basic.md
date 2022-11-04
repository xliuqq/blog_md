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





## Spring 全局资源的加载

实现CommandLIneRunner 或者 ApplicationRunner，实现 run 方法






