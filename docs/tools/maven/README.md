# MAVEN

## 插件

- [常用的插件](plugins.md)

## 参数

```shell
# -T多线程 1C 一核一线程
mvn clean -DskipTests -T 1C package  
```

## 生命周期

[ validate, **initialize**, generate-sources, process-sources, generate-resources, process-resources, **compile**, process-classes, generate-test-sources, process-test-sources, generate-test-resources, process-test-resources, test-compile, process-test-classes, **test**, prepare-package, **package**, pre-integration-test, integration-test, post-integration-test, **verify**, install, deploy ]

### 插件执行顺序

**maven对于绑定到同一phase上的多个插件的执行顺序是按照它们在pom.xml声明的顺序来的**。

如果在父 pom 中定义，或者在 profile 的 plugins 定义，需要在子pom中重新声明，以指定执行顺序。

**Parent pom.xml**

```xml
<plugins>
    <plugin>
        <groupId>groupid.maven.1</groupId>
        <artifactId>maven-plugin-1</artifactId>
        <version>1.0</version>
        <executions>
            <execution>
                <phase>package</phase>
            </execution>
        </executions>
    </plugin>
</plugins>
```

**Child pom.xml**

```xml
<plugins>
    <plugin>
        <groupId>groupid.maven.2</groupId>
        <artifactId>maven-plugin-2</artifactId>
        <version>1.0</version>
        <executions>
            <execution>
                <phase>package</phase>
            </execution>
        </executions>
    </plugin>
    <plugin>
        <groupId>groupid.maven.1</groupId>
        <artifactId>maven-plugin-1</artifactId>
    </plugin>
</plugins>
```

## 配置

### 依赖管理

冲突解决

在配置文件pom.xml中先声明要使用哪个版本的相应jar包，声明后其他版本的jar包一律不依赖。（type和import表示从该pom文件中继承依赖管理）

```xml 
<properties>
    <spring.version>4.2.4.RELEASE</spring.version>
    <hibernate.version>5.0.7.Final</hibernate.version>
    <struts.version>2.3.24</struts.version>
</properties>
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>com.cbm.stu</groupId>
            <artifactId>maven-parent-a</artifactId>
            <version>1.0.0</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
```

## 构建

### Maven单独构建多模块项目中的单个模块

说明：

1、可能存在的场景，多模块项目没有互相引用，那么此时可以单独构建单个项目，指定到子模块的pom.xml文件即可完成编译。

2、如果多模块项目各自都引用了，那么单独编译子模块的pom.xml文件会直接报错，解决方法就是编译父项目pom.xml。

3、如果编译父项目，那么可能会造成编译时间很慢，其中有些项目也不需要编译，解决方法如下：

Maven选项：

```
-pl, --projects
        Build specified reactor projects instead of all projects
-am, --also-make
        If project list is specified, also build projects required by the list
-amd, --also-make-dependents
        If project list is specified, also build projects that depend on projects on the list
```

首先切换到工程的根目录

单独构建模块jsoft-web，同时会构建jsoft-web模块依赖的其他模块

```
mvn install -pl jsoft-web -am
```

单独构建模块jsoft-common，同时构建依赖模块jsoft-common的其他模块 

```
mvn install -pl jsoft-common -am -amd
```

### Resources

- 默认会把src/main/resources下的文件和class文件一起打包到jar内部，可以通过

```xml
<build>
    <resources>  
        <resource>  
            <directory>src/main/resources</directory>  
            <includes>  
                <include>*.txt</include>  
            </includes>  
            <excludes>  
                <exclude>*.xml</exclude>  
            </excludes>  
        </resource>  
        <!-- 把src/main/resources目录下所有的文件拷贝到conf目录中 -->
        <resource>
            <directory>src/main/resources</directory>
            <targetPath>${project.build.directory}/conf</targetPath>
        </resource>
        <!-- 把lib目录下所有的文件拷贝到lib目录中
            （可能有些jar包没有办法在maven中找到，需要放在lib目录中） -->
        <resource>
            <directory>lib</directory>
            <targetPath>${project.build.directory}/lib</targetPath>
        </resource>
        <!-- 把放在根目录下的脚本文件.sh,.bat拷贝到bin目录中 -->
        <resource>
            <directory>.</directory>
            <includes>
                <include>**/*.sh</include>
                <include>**/*.bat</include>
            </includes>
            <targetPath>${project.build.directory}/bin</targetPath>
        </resource>
    </resources>  
</build>
```

