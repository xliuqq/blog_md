# swagger

https://github.com/swagger-api/swagger-codegen

- 支持json和yaml文件解析，自动生成多种语言的API客户端和服务器stub；
- swagger配置规范说明，https://swagger.io/specification；
- swagger maven plugin，https://github.com/garethjevans/swagger-codegen-maven-plugin；

![OpenAPI2.0 OpenAPI3.0 info](pics/swagger.png)



## springboot配置

```xml
<dependency>
    <groupId>io.springfox</groupId>
    <artifactId>springfox-swagger2</artifactId>
    <version>2.7.0</version>
</dependency>

<dependency>
    <groupId>io.springfox</groupId>
    <artifactId>springfox-swagger-ui</artifactId>
    <version>2.7.0</version>
</dependency>
```



Swagger配置

```java
@Configuration
@EnableSwagger2
public class SwaggerConfig{

    @Bean
    public Docket createRestApi() {
        return new Docket(DocumentationType.SWAGGER_2)
                .apiInfo(apiInfo())
                .select()
                .apis(RequestHandlerSelectors.basePackage("com.itguang.springbootswaggerdemo1.web"))
                .paths(PathSelectors.any())
                .build();
    }

    private ApiInfo apiInfo() {
        return new ApiInfoBuilder()
                .title("Spring Boot中使用Swagger2构建RESTful API")
                .description("rest api 文档构建利器")
                .termsOfServiceUrl("http://blog.csdn.net/itguangit")
                .contact("itguang")
                .version("1.0")
                .build();
    }

}
```

