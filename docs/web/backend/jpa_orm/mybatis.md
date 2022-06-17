## Mybatis



## 基础

`#`和`$`的区别：

- `#`将传入的数据都当成一个字符串，会对自动传入的数据加一个双引号。如：`order by #user_id#`，如果传入的值是111,那么解析成sql时的值为`order by "111"`, 如果传入的值是id，则解析成的sql为`order by "id"`.
- `$`将传入的数据直接显示生成在sql中。如：`order by $user_id$`，如果传入的值是user_id，那么解析成sql时的值为`order by user_id`, 如果传入的值是id，则解析成的sql为`order by id`.
- `#`方式能够很大程度防止sql注入，`$`方式无法防止Sql注入。
- `$`方式一般用于传入数据库对象，例如传入表名。
- 一般能用`#`的就别用`$`.



## 示例

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
<mapper namespace="com.test.mapper.IndividualCustomerMapper">
    <update id="updateAttributeGrayByID" parameterType="com.test.entity.CustomerDO">
        UPDATE t_individualcustomer
    	SET IsGray = #{isGray}
    	WHERE CustomerID IN
            <foreach collection="customerID" item="smid" index="index" open="(" separator="," close=")">
          		#{smid}
            </foreach>
    </update>
</mapper>
```



### Springboot配置

```xml
<dependency>
    <groupId>org.mybatis.spring.boot</groupId>
    <artifactId>mybatis-spring-boot-starter</artifactId>
    <version>2.1.3</version>
</dependency>
```

```yaml
# application.yml
mybatis:
    type-aliases-package: com.example.domain.model
    type-handlers-package: com.example.typehandler
    configuration:
        map-underscore-to-camel-case: true
        default-fetch-size: 100
        default-statement-timeout: 30
...
```



Mybatis的具体配置

| Property                                 | Description                                                  |
| :--------------------------------------- | :----------------------------------------------------------- |
| `config-location`                        | Location of MyBatis xml config file.                         |
| `check-config-location`                  | Indicates whether perform presence check of the MyBatis xml config file. |
| `mapper-locations`                       | Locations of Mapper xml config file.                         |
| `type-aliases-package`                   | Packages to search for type aliases. (Package delimiters are “`,; \t\n`”) |
| `type-aliases-super-type`                | The super class for filtering type alias. If this not specifies, the MyBatis deal as type alias all classes that searched from `type-aliases-package`. |
| `type-handlers-package`                  | Packages to search for type handlers. (Package delimiters are “`,; \t\n`”) |
| `executor-type`                          | Executor type: `SIMPLE`, `REUSE`, `BATCH`                    |
| `default-scripting-language-driver`      | The default scripting language driver class. This feature requires to use together with mybatis-spring 2.0.2+. |
| `configuration-properties`               | Externalized properties for MyBatis configuration. Specified properties can be used as placeholder on MyBatis config file and Mapper file. For detail see the [MyBatis reference page](http://www.mybatis.org/mybatis-3/configuration.html#properties). |
| `lazy-initialization`                    | Whether enable lazy initialization of mapper bean. Set `true` to enable lazy initialization. This feature requires to use together with mybatis-spring 2.0.2+. |
| `mapper-default-scope`                   | Default scope for mapper bean that scanned by auto-configure. This feature requires to use together with mybatis-spring 2.0.6+. |
| `configuration.*`                        | Property keys for `Configuration` bean provided by MyBatis Core. About available nested properties see the [MyBatis reference page](http://www.mybatis.org/mybatis-3/configuration.html#settings). **NOTE**: This property cannot be used at the same time with the `config-location`. |
| `scripting-language-driver.thymeleaf.*`  | Property keys for `ThymeleafLanguageDriverConfig` bean provided by MyBatis Thymeleaf. About available nested properties see the [MyBatis Thymeleaf reference page](http://www.mybatis.org/thymeleaf-scripting/user-guide.html#_configuration_properties). |
| `scripting-language-driver.freemarker.*` | Properties keys for `FreeMarkerLanguageDriverConfig` bean provided by MyBatis FreeMarker. About available nested properties see the [MyBatis FreeMarker reference page](http://www.mybatis.org/freemarker-scripting/#Configuration). This feature requires to use together with mybatis-freemarker 1.2.0+. |
| `scripting-language-driver.velocity.*`   | Properties keys for `VelocityLanguageDriverConfig` bean provided by MyBatis Velocity. About available nested properties see the [MyBatis Velocity reference page](http://www.mybatis.org/velocity-scripting/#Configuration). This feature requires to use together with mybatis-velocity 2.1.0+. |