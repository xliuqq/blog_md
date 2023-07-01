# DB 操作

## JPA与ORM

**ORM(Object Relation Mapping)** 思想是建立实体类和数据库元数据的自动映射关系，减少数据库访问层的代码编写；

**JPA(Java Persistence API) 是Java EE5.0的平台标准的ORM规范，Hibernate兼容**。

- @Entity代表是一个实体，表示与数据库有映射关系；
- @Table表示与之对应的数据表，可以看到类属性与数据表的列是顺序对应的关系，属性上的注解表明了列的类型与状态；
- @Column表示是表中的某个字段；
- @Id表明是主键。当然也可以不使用注解，在xml中配置；

- @Transient表示成员不是数据库中的字段；



## Mybatis

> 通过 SQL 操作

```sql
# Mysql 不管用=还是!=都是会把null过滤掉的，只能通过 is null 或 is not null
id != ''
```

### 基础

`#`和`$`的区别：

- `#`将传入的数据都当成一个字符串，会对自动传入的数据加一个双引号。如：`order by #user_id#`，如果传入的值是111,那么解析成sql时的值为`order by "111"`, 如果传入的值是id，则解析成的sql为`order by "id"`.
- `$`将传入的数据直接显示生成在sql中。如：`order by $user_id$`，如果传入的值是user_id，那么解析成sql时的值为`order by user_id`, 如果传入的值是id，则解析成的sql为`order by id`.
- `#`方式能够很大程度防止sql注入，`$`方式无法防止Sql注入。
- `$`方式一般用于传入数据库对象，例如传入表名。
- 一般能用`#`的就别用`$`.



### 示例

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



**Springboot配置**

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



**Mybatis** 的具体配置

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



## Mybatis-plus

> 用于单表操作

### 返回自增主键

实体对象 主键`IdType`要设置为`AUTO` 表示数据库ID自增，然后使用Mapper进行插入时，返回的实体就会包含主键值。

```java
// employee 在插入后，就会自动插入主键到对象中
employeeService.saveOrUpdate(employee);
baseMapper.insert(employee);
```



### 字段为null时不更新

下面的方法，不会对实体中值为Null的属性（字段）进行更新。

```java
mapper.updateById(entity); 
```

Mybatis-plus默认配置中，对于参数字段为null时，会自动忽略。导致进行update操作时，无法将字段更新为null值。

- 单个配置，每个参数上加注解。（但是如果其它场景不传入该值时，会导致会被更新，可能会出问题）

  ```java
  @TableField(updateStrategy = FieldStrategy.IGNORED)
  ```

- 使用**LambdaUpdateWrapper**（推荐）

  ```java
  LambdaUpdateWrapper<BizFile> lambdaUpdateWrapper = new LambdaUpdateWrapper<>();
  //过滤条件
  lambdaUpdateWrapper.eq(BizFile::getId, bizFile.getId());
  
  //下面为设置值  		
  //由于parentId会为空，所以要使用LambdaUpdateWrapper
  lambdaUpdateWrapper.set(BizFile::getParentId, parentId);
  lambdaUpdateWrapper.set(BizFile::getPath, newDirPath);
  
  //更新
  this.update(null, lambdaUpdateWrapper);
  ```

  

## Mybatis-plus-join

> https://github.com/yulichang/mybatis-plus-join
>
> 多表连接的方案。



## [BeanSearch](https://bs.zhxu.cn/)

> 专注高级查询的**只读 ORM，天生支持联表**，免 DTO/VO 转换，使一行代码实现复杂列表检索成为可能！
>
> - **推荐** 在 **非事务性** 的 **动态** 检索场景中使用：如管理后台的订单查询等；
>
> - **不建议** 在 **事务性** 的 **静态** 查询场景中使用：如用户注册接口的查询账号已存在的场景；

Bean Searcher 是一个**轻量级数据库条件检索引擎**，它的作用是**从已有的数据库表中检索数据**，它的目的是为了减少后端模板代码的开发，极大提高开发效率，节省开发时间，使得一行代码完成一个列表查询接口成为可能！

- 不依赖具体的 Web 框架（即可以在任意的 Java Web 框架内使用）
- 不依赖具体的 ORM 框架（即可以与任意的 ORM 框架配合使用，没有 ORM 也可单独使用）

### 架构

![img](pics/beansearch_architecture.jpg)

### 与 Hibernate MyBatis 的区别

首先，Bean Searcher 并不是一个完全的 `ORM` 框架，弥补他们在 `列表检索领域` 的不足。

| 区别点           | Bean Searcher | Hibernate  | MyBatis    |
| ---------------- | ------------- | ---------- | ---------- |
| ORM              | **只读** ORM  | 全自动 ORM | 半自动 ORM |
| 实体类可多表映射 | 支持          | 不支持     | 不支持     |
| 字段运算符       | **动态**      | 静态       | 静态       |
| CRUD             | Only R        | CRUD       | CRUD       |



## 刷库

### 作用

1. 维护管理数据库的版本信息，记录当前服务数据库版本。
2. 支持实现连续多版本自动升级。
3. 对数据库操作进行控制，可将多个数据库操作声明成事务。

### [开源Flyway](https://flywaydb.org/documentation/)



#### 介绍

1. 可执行数据库脚本语句，进行数据库升级
2. 管理数据版本，记录当前数据的版本信息
3. 版本升级支持事务，一个版本若失败则回滚
4. 刷库需要在服务停止的情况下进行



### 自定义实现

#### 核心业务实现逻辑

- 从sql_version.json 中获取所有的版本号及对应的脚本路径
- 读取项目的数据库连接配置，连接数据库
- 从数据库中读取当前的版本号
- 如果version表不存在，则创建version表，设置初始版本0.0，否则，获取version表的版本号字段对应的值
- 循环判断 sql_version.json 中获取所有的版本号
- 如果json版本号 > 表中的版本号，则按序执行对应的sql语句，如3.0版本的时候，不需要执行2.0的sql
- sql执行全部成功，则返回true，否则以异常的形式抛出

#### 刷库规范定义

bin目录下建upgrade_db.sh文件

```shell
#!/usr/bin/env bash

app_class_name=com.hongcheng.aibigdata.userservice.UserServiceApplication
pid=`jps -vm |grep ${app_class_name} |awk '{print $1}'`
BASE_HOME=$(cd `dirname $0`/../; pwd)

db_class=com.hongcheng.aibigdata.common.db.Upgrade
if [ $pid ]
then
echo "海星服务正在运行，请停止服务后进行刷库"
else
# 单独通过java执行
SQL_PATH=$BASE_HOME/scripts/sql_version.json

java -Dbase.dir=${BASE_HOME} -cp BASE_HOME/lib/*:BASE_HOME/conf/. -Dloader.main=${db_class} org.springframework.boot.loader.PropertiesLauncher $SQL_PATH

if [ $? -eq 0 ]; then
echo "刷库成功"
else
echo "刷库失败，请查看具体异常"
fi
fi
```

集成common.util包

核心代码如下：

```java
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.nio.charset.StandardCharsets;
import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.List;
import java.util.Properties;
import com.hongcheng.aibigdata.common.util.JsonUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.config.YamlPropertiesFactoryBean;
import org.springframework.core.io.ClassPathResource;
import org.springframework.core.io.FileSystemResource;
import org.springframework.core.io.Resource;
import org.springframework.util.FileCopyUtils;

/**
* 数据库升级脚本执行
*/
public class Upgrade {
    private static final Logger LOG = LoggerFactory.getLogger(Upgrade.class);
    
    /**
    * Version表只有两个字段（property和value组成，长度限制为 char(64)
    */
    private static final String VERSION_TABLE_NAME = "version";
    
    private static final String APPLICATION_PATH = "application.yml";
    
    /**
    * 参数格式： --script-path $SCRIPT_HOME
    */
    public static void main(String[] args) {
        // 获取脚本的根目录
        String sqlInfoJsonPath = args[0];
        LOG.info("***********************************sql flush appliction start***********************************");
        LOG.info("SQL description file is {}", sqlInfoJsonPath);
        // 进行sql升级
        try {
            //传入StarFish HOME路径
            new Upgrade().upgrade(sqlInfoJsonPath);
        } catch (SQLException e) {
            LOG.error("sql error", e);
        } catch (Exception e) {
            LOG.error("execute error:", e);
        }
    }
    
    private Connection getConnection() {
        YamlPropertiesFactoryBean yaml = new YamlPropertiesFactoryBean();
        yaml.setResources(new ClassPathResource(APPLICATION_PATH));
        Properties properties = yaml.getObject();
        if (properties == null) {
            LOG.error("init dataSource error , please check the config yml");
            throw new NullPointerException("init dataSource properties error");
        }
        try {
            Class.forName(properties.getProperty("spring.datasource.driver-class-name"));
            return DriverManager.getConnection(properties.getProperty("spring.datasource.url"),
                                               properties.getProperty("spring.datasource.username"),
                                               properties.getProperty("spring.datasource.password"));
        } catch (ClassNotFoundException | SQLException e) {
            LOG.error("Find sql driver failed for:{}, exception: {}", properties.getProperty("spring.datasource.driver-class-name"), e);
            System.exit(-1);
        }
        return null;
    }
    
    public boolean upgrade(String sqlInfoJsonPath) throws SQLException, IOException {
        try (Connection conn = getConnection()) {
            //从json中获取所有的版本号及对应的脚本路径
            VersionJsonEntity versionJsonEntity = getVersionJsonEntity(sqlInfoJsonPath);
            if (versionJsonEntity == null) {
                return false;
            }
            List<VersionScriptEntity> versionScriptEntities = versionJsonEntity.getData();
            
            // 2. 从数据库中读取当前的版本号
            String versionDb = "0.0";
            
            //SQL语句具备重复执行不报错，不需要设置autoCommit
            if (!existVersion(conn, VERSION_TABLE_NAME)) {
                // 2.1 如果version表不存在，则创建version表，设置初始版本0.0
                createVersionTable(conn);
                insertVersion(conn, versionDb);
            } else {
                // 2.2 否则，获取version表的版本号字段对应的值
                versionDb = getVersion(conn);
            }
            LOG.info("current database version is {}", versionDb);
            //for循环判断（3.0的时候，不需要执行2.0的sql）
            for (VersionScriptEntity versionScriptEntity : versionScriptEntities) {
                String versionValue = versionScriptEntity.getVersion();
                List<String> sqlPaths = versionScriptEntity.getSql();
                if (compareVersionNum(versionValue, versionDb)) {
                    // 3. 如果json版本号 > 表中的版本号，则执行对应的sql语句
                    LOG.info("start to init database version : {}", versionValue);
                    run(conn, sqlPaths, sqlInfoJsonPath);
                    updateVersion(conn, versionValue);
                    versionDb = versionValue;
                }
            }
            LOG.info("update sql finish");
            // sql执行全部成功，则返回true，否则以异常的形式抛出
        }
        
        return true;
    }
    
    private String loadJson(String fileName) throws IOException {
        LOG.info("load json file {}", fileName);
        File file = new File(fileName);
        Resource resource = new FileSystemResource(file);
        byte[] bytes;
        try {
            bytes = FileCopyUtils.copyToByteArray(resource.getInputStream());
            return new String(bytes);
        } catch (IOException e) {
            LOG.error("load json error {}:", fileName);
            throw e;
        }
        
    }
    
    
    private VersionJsonEntity getVersionJsonEntity(String jsonPath) throws IOException {
        String jsonString = loadJson(jsonPath);
        //采用Java类解析，不解析成JsonNode
        return JsonUtil.decode(jsonString, VersionJsonEntity.class);
    }
    
    /**
    * 判断是否存在version表
    */
    private boolean existVersion(Connection conn, String tableName) throws SQLException {
        boolean flag;
        DatabaseMetaData meta = conn.getMetaData();
        String[] type = {"TABLE"};
        String catalog = conn.getCatalog();
        LOG.info("catalog is {} ", conn.getCatalog());
        ResultSet rs = meta.getTables(catalog, null, tableName, type);
        flag = rs.next();
        LOG.info("check version is :{}", flag);
        return flag;
        
    }
    
    /**
    * 获取版本号
    */
    private String getVersion(Connection conn) throws SQLException {
        //预编译语句
        // where property=version的条件（防止万一后面version表加其它的行）
        String sql = "select value from version where property='version'";
        try (PreparedStatement statement = conn.prepareStatement(sql)) {
            ResultSet rs = statement.executeQuery();
            // 4.处理数据库的返回结果(使用ResultSet类)
            if (rs.next()) {
                return rs.getString("value");
            }
            
        } catch (SQLException e) {
            LOG.error("get version table error");
            throw e;
        }
        return "";
    }
    
    /**
    * 更新版本号
    */
    private boolean updateVersion(Connection conn, String value) throws SQLException {
        //预编译语句
        String sql = "update version set value=? where property = 'version'";
        try (PreparedStatement statement = conn.prepareStatement(sql)) {
            statement.setString(1, value);
            int i = statement.executeUpdate();
            return i == 1;
        } catch (SQLException e) {
            LOG.error("update version table error");
            throw e;
        }
    }
    
    /**
    * 插入版本号
    */
    private boolean insertVersion(Connection conn, String value) throws SQLException {
        //预编译语句
        String sql = "insert into version(property,value) values('version',?)";
        try (PreparedStatement statement = conn.prepareStatement(sql)) {
            statement.setString(1, value);
            int i = statement.executeUpdate();
            return i == 1;
        } catch (SQLException e) {
            LOG.error("insert version table error");
            throw e;
        }
        
    }
    
    /**
    * 建version表
    */
    private boolean createVersionTable(Connection conn) throws SQLException {
        LOG.info("create version table");
        String sql = "CREATE TABLE version(" +
            " `property` VARCHAR(64) NOT NULL," +
            " `value` VARCHAR(64) NOT NULL," +
            "PRIMARY KEY (`property`));";
        try (Statement statement = conn.createStatement()) {
            int i = statement.executeUpdate(sql);
            return i == 1;
        } catch (SQLException e) {
            LOG.error("create version table error");
            throw e;
        }
    }
    
    
    /**
    * 运行指定的sql脚本
    *
    * @param sqlPaths 需要执行的sql脚本的路径集合
    */
    private boolean run(Connection conn, List<String> sqlPaths, String sqlPathHome) throws IOException {
        // 创建ScriptRunner，用于执行SQL脚本
        ScriptRunner runner = new ScriptRunner(conn);
        runner.setErrorLogWriter(null);
        runner.setLogWriter(null);
        // 执行SQL脚本
        for (String sqlPath : sqlPaths) {
            String filePath = sqlPathHome + sqlPath;
            File file = new File(filePath);
            Resource resource = new FileSystemResource(file);
            Reader inputStreamReader = new InputStreamReader(resource.getInputStream(), StandardCharsets.UTF_8);
            runner.runScript(inputStreamReader);
            LOG.info("excute sql ：{}", sqlPath);
        }
        return true;
    }
    
    
    /**
    * 比较版本
    *
    * @return true : first > second, otherwise false
    */
    public boolean compareVersionNum(String versionValue, String versionDb) {
        if (versionValue == null || versionDb == null) {
            return false;
        }
        String[] versionChars = versionValue.split("\\.");
        String[] dbChars = versionDb.split("\\.");
        for (int i = 0; i < versionChars.length; i++) {
            int versionInt = Integer.parseInt(versionChars[i]);
            int versionDbInt = Integer.parseInt(dbChars[i]);
            if (versionInt > versionDbInt) {
                return true;
            } else if (versionInt < versionDbInt) {
                return false;
            }
        }
        return false;
    }
}
```

**新建script文件夹**

目录结构如下：

![img](pics/1647307054474-5b18a268-046d-4372-ac0a-af4276aca4cd.png)

sql_version.json

```json
{
  "data": [
    {
      //按版本号执行
      "version": "1.0",
      "sql": [
        //按序执行对应地址的sql脚本
        "/v1.0/001-createDataBase.sql",
        "/v1.0/002-initDataBase.sql"
      ]
    }
  ]
}
```

对应实体类如下：

```java
public class VersionJsonEntity {

    private List<VersionScriptEntity> data;

    public List<VersionScriptEntity> getData() {
        return data;
    }

    public void setData(List<VersionScriptEntity> data) {
        this.data = data;
    }

}
      
public class VersionScriptEntity {
    private String version;
    private List<String> sql;

    public String getVersion() {
        return version;
    }

    public void setVersion(String version) {
        this.version = version;
    }

    public List<String> getSql() {
        return sql;
    }

    public void setSql(List<String> sql) {
        this.sql = sql;
    }
}
```

