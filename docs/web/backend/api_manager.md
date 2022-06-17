# 接口管理设计（Spring Boot）

## 需求

微服务的不同版本间，可以查看接口的改动。



## 基本思路

1. 每个版本都有对应的 OpenAPI 规范的 Yaml 描述文件，通过[OpenAPI Tools](https://github.com/OpenAPITools/openapi-diff)进行接口版本比对；

每个版本如何具备对应的OpenAPI的yaml描述文件？

## 思路1：根据Yaml生成Controller层注解代码

> 开发接口先写OpenAPI 3 的 yaml 文件，然后生成对应的Controller层的接口。

工具

- [openapi-generator和maven-plugin](https://github.com/OpenAPITools/openapi-generator): 根据Yaml生成客户端/服务端的桩代码；

问题点:

- openapi-generator 生成的代码和其它配置比较多，因此需要深入研究参数，如何屏蔽一些无用的配置和代码；
  - `.openapi-genrator`目录、`.openapi-generator-ignore`文件，`applicaiton.properties`, `openapi.yaml` 和 `org.openapitools`包
- 改动OpenAPI的yaml，需要重新生成相关的api/pojo代码，即生成的Controller代码不能进行修改；

### 实战代码

`pom.xml`配置，具体代码见 []()

```xml
<dependency>
    <groupId>org.springdoc</groupId>
    <artifactId>springdoc-openapi-ui</artifactId>
    <version>1.6.6</version>
</dependency>
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
    <exclusions>
        <exclusion>
            <artifactId>logback-classic</artifactId>
            <groupId>ch.qos.logback</groupId>
        </exclusion>
    </exclusions>
</dependency>
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-test</artifactId>
</dependency>
<dependency>
    <groupId>com.google.code.findbugs</groupId>
    <artifactId>jsr305</artifactId>
    <version>3.0.2</version>
</dependency>
<dependency>
    <groupId>com.fasterxml.jackson.dataformat</groupId>
    <artifactId>jackson-dataformat-yaml</artifactId>
</dependency>
<dependency>
    <groupId>com.fasterxml.jackson.datatype</groupId>
    <artifactId>jackson-datatype-jsr310</artifactId>
</dependency>
<dependency>
    <groupId>org.openapitools</groupId>
    <artifactId>jackson-databind-nullable</artifactId>
    <version>0.2.2</version>
</dependency>
<dependency>
    <groupId>org.openapitools</groupId>
    <artifactId>openapi-generator</artifactId>
    <version>${openapi-generator-version}</version>
</dependency> 

... 

<plugin>
     <groupId>org.openapitools</groupId>
     <artifactId>openapi-generator-maven-plugin</artifactId>
     <version>${openapi-generator-version}</version>
     <executions>
         <execution>
             <goals>
                 <goal>generate</goal>
             </goals>
             <configuration>
                 <inputSpec>${project.basedir}/src/main/resources/swagger/petastore.yaml</inputSpec>
                 <generatorName>spring</generatorName>
                 <apiPackage>com.xliu.lifelearning.cs.spring.springbootopenapigenerator.api</apiPackage>
                 <modelPackage>com.xliu.lifelearning.cs.spring.springbootopenapigenerator.pojo</modelPackage>
                 <skipValidateSpec>false</skipValidateSpec>
                 <!--> 生成在项目源码中，而不是在target/classes目录下<-->
                 <output>${project.basedir}</output>
                 <generateApiDocumentation>false</generateApiDocumentation>
                 <!--> 禁止重写已经存在的文件，如果接口有变动，则将文件删除重新生成<-->
                 <skipOverwrite>true</skipOverwrite>
                 <library>spring-boot</library>

                 <configOptions>
                     <delegatePattern>false</delegatePattern>
                     <title>swagger</title>
                     <serializableModel>true</serializableModel>
                 </configOptions>
             </configuration>
         </execution>
     </executions>
</plugin>
```



## **思路2：根据Controller**层注解代码生成Yaml

工具：

- [springdoc-openapi](https://github.com/springdoc/springdoc-openapi) : 在SpringBoot中使用OpenAPI 3的注解；

- [springdoc-openapi-maven-plugin](https://github.com/springdoc/springdoc-openapi-maven-plugin) : 根据 OpenAPI 3的注解生成OpenAPI 3的Yaml

问题点：

- 在**运行时才能生成OpenAPI 文档**（通过`mvn verify`）：`springdoc-openapi-maven-plugin` 依赖于 `spring-boot-maven` 插件。Maven在**集成测试阶段**(integration-test)运行openapi插件。

```xml
<dependency>
    <groupId>org.springdoc</groupId>
    <artifactId>springdoc-openapi-ui</artifactId>
    <version>1.6.6</version>
</dependency>

 <plugin>
     <groupId>org.springframework.boot</groupId>
     <artifactId>spring-boot-maven-plugin</artifactId>
     <version>2.3.4.RELEASE</version>
     <configuration>
         <jvmArguments>-Dspring.application.admin.enabled=true</jvmArguments>
     </configuration>
     <executions>
         <execution>
             <id>pre-integration-test</id>
             <goals>
                 <goal>start</goal>
             </goals>
         </execution>
         <execution>
             <id>post-integration-test</id>
             <goals>
                 <goal>stop</goal>
             </goals>
         </execution>
     </executions>
</plugin>
<plugin>
    <!-- springdoc-openapi-maven-plugin 依赖于 spring-boot-maven 插件.
                 Maven在集成测试阶段(integration-test)运行openapi插件。-->
    <groupId>org.springdoc</groupId>
    <artifactId>springdoc-openapi-maven-plugin</artifactId>
    <version>1.1</version>
    <configuration>
        <outputDir>${project.basedir}/src/main/resources/</outputDir>
        <outputFileName>openapi.json</outputFileName>
    </configuration>
    <executions>
        <execution>
            <id>integration-test</id>
            <goals>
                <goal>generate</goal>
            </goals>
        </execution>
    </executions>
</plugin>
```



