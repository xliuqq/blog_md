[toc]

# JSON Processor

> scala和Java读取json文件，使用[Jackson](https://github.com/FasterXML/jackson-core)包；



## FasterXML Jackson

github地址：https://github.com/FasterXML/jackson

一般会直接使用data-bind，将JSON与Java对象进行转换，https://github.com/FasterXML/jackson-databind

Maven地址

```xml
<dependency>
    <groupId>com.fasterxml.jackson.core</groupId>
    <artifactId>jackson-databind</artifactId>
    <version>${jackson.version.core}</version>
</dependency>
```

### 支持的数据格式

[text](https://github.com/FasterXML/jackson-dataformats-text)：JSON, CSV, Java Properties, YAML, XML

[binary](https://github.com/FasterXML/jackson-dataformats-binary)：Avro, BSON(binary json), CBOR, Smile, **Protobuf**, 

```xml
<!-- 数据格式的jar包引入 -->
<dependency>
  <groupId>com.fasterxml.jackson.dataformat</groupId>
  <artifactId>jackson-dataformat-yaml</artifactId>
  <version>2.9.2</version>
</dependency>
```

### 使用

```java
ObjectMapper mapper = new ObjectMapper(new YAMLFactory());  // different factory
User user = mapper.readValue(yamlSource, User.class);
// 带泛型的反序列化
Result<List<AfsStatus>>> tmp = mapper.readValue(yamlSource, new TypeReference<Result<List<AfsStatus>>>() {});
// 或者
YAMLFactory factory = new YAMLFactory();
YAMLParser parser = factory.createParser(yamlString);
while (parser.nextToken() != null) {
  // do something!
}
```

### Projects

#### Core Modules

- Streaming("jackson-core"): 定义底层的streaming API，包含JSON的实现；

- Annotations("jackson-annotations"): 标准的Jackson注解；

- Databind("jackson-databind")：实现data-binding和对象序列化，基于上面两个包实现；

#### Else

- JVM语言：[Kotlin](https://github.com/FasterXML/jackson-module-kotlin) 包，[Scala](https://github.com/FasterXML/jackson-module-scala) 包；
- 数据格式：见[支持的数据格式](#支持的数据格式)；
- 数据类型：支持常用的，标准的数据类型的映射转换
  - 官方：[Collection类](https://github.com/FasterXML/jackson-datatypes-collections)，[Hibernate](https://github.com/FasterXML/jackson-datatype-hibernate)，[Java 8 Modules](https://github.com/FasterXML/jackson-modules-java8) 等；

### 读取方式

支持

- Stream: JsonFactory（线程安全，一次性配置）类读取，low-level；
- **databind**: ObjectMapper（线程安全） 类读取；
  - 对象映射，POJO对象、Map、List等；
  - Tree Model：适用于结构动态变换，难以用java对象简单表示；
  - Streaming：mapper.getFactory转换到流式处理，low level；

#### 自定义类解析

```java
ObjectMapper objectMapper = new ObjectMapper()
SimpleModule module = new SimpleModule();
module.addDeserializer(Schema.class, new SchemaDeserializer());
objectMapper.registerModule(module);
```

#### 多态序列化

```java
@JsonTypeInfo(use = JsonTypeInfo.Id.NAME, include = JsonTypeInfo.As.PROPERTY,property = "name")
@JsonSubTypes(value = {
     @JsonSubTypes.Type(value = Daedalus.class, name = "Daedalus"),
     @JsonSubTypes.Type(value = HugeDrink.class, name = "Huge drink"),
     @JsonSubTypes.Type(value = StarWand.class, name = "Star wand"),
})
public interface Equipment {
}

@Data
public class StarWand implements Equipment{
    private String name;
    private int length;
    private int price;
    private List<String> effect;
}

// @JsonSubTypes的注解,可以通过代码操作
mapper.registerSubtypes(new NamedType(HugeDrink.class, "Huge drink"));
mapper.registerSubtypes(new NamedType(Daedalus.class, "Daedalus"));
mapper.registerSubtypes(new NamedType(StarWand.class, "Star wand"));

```

新增子类的时候都要去加一下JsonSubTypes，通过 `@JsonTypeIdResolver` 解决

```java
@JsonTypeInfo(use = JsonTypeInfo.Id.CUSTOM, property = "type")
@JsonTypeIdResolver(SchemaIdResolver.class)
public interface Equipment {
}

public class SchemaIdResolver extends TypeIdResolverBase {
}

// 处理不存在的 type 或者 丢失时，如何处理
objectMapper.setConfig( objectMapper.getDeserializationConfig().withHandler(new DeserializationProblemHandler() {
            @Override
            public JavaType handleMissingTypeId(DeserializationContext ctxt, JavaType baseType, TypeIdResolver idResolver, String failureMsg) throws IOException {
                return baseType;
            }

            @Override
            public JavaType handleUnknownTypeId(DeserializationContext ctxt, JavaType baseType, String subTypeId, TypeIdResolver idResolver, String failureMsg) throws IOException {
                return baseType;
            }
        }));

```

#### 枚举

```java
// 使用 ENUM 的 toString 进行序列化和反序列化
objectMapper.configure(SerializationFeature.WRITE_ENUMS_USING_TO_STRING, true);
objectMapper.configure(DeserializationFeature.READ_ENUMS_USING_TO_STRING, true);
```

## JsonPath

[JsonPath](https://github.com/json-path/JsonPath) ：根据路径解析所需的数据

```xml
<dependency>
    <groupId>com.jayway.jsonpath</groupId>
    <artifactId>json-path</artifactId>
    <version>2.4.0</version>
</dependency>
```

通过SPI机制，支持Gson，FastXML jackson 等json解析工具。



## FastJSON

阿里的 json工具，同时支持 json 解析和 [JSONPath](https://github.com/alibaba/fastjson/wiki/JSONPath) 。

