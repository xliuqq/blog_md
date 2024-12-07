[toc]

# XML

## 格式

**xmlns** 即 XML NameSpace，名空间，前缀来防止标签冲突；默认名空间，不需要加前缀；

**xmlns:mvc** 表示mvc名空间。

**xsi:schemaLocation** 定义了**XML Namespace和对应的 XSD（Xml Schema Definition）文档的位置的关系**；

- 第一个URI是定义的 XML Namespace的值；
- 第二个URI给出Schema文档的位置，Schema处理器将从这个位置读取Schema文档，该文档的targetNamespace必须与第一个URI相匹配。

xsi 已经成为了一个业界默认的用于 XSD（XML Schema Definition) 文件的命名空间。 

XSD 文件（XML Schema Definition，也常常称为 Schema 文件）是用来定义 xml 文档结构的。

- XSD 校验XML文件内容的合法性，一个 XML 配置文件是合法的 XML 文件格式， 但其不一定符合该 XML 的 XSD 模式。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:mvc="http://www.springframework.org/schema/mvc"
       xsi:schemaLocation="http://www.springframework.org/schema/beans 
                           http://www.springframework.org/schema/beans/spring-beans.xsd
                           http://www.springframework.org/schema/context 
                           http://www.springframework.org/schema/context/spring-context.xsd
                           http://www.springframework.org/schema/mvc
                           http://www.springframework.org/schema/mvc/spring-mvc.xsd">
    <context:component-scan base-package="xxx.xxx.controller" />
     
    <context:annotation-config/>
    <mvc:default-servlet-handler/>
    <mvc:annotation-driven/>
     
    <mvc:resources mapping="/images/**" location="/images/" />
     
    <bean id="xxx" class="xxx.xxx.xxx.Xxx">
        <property name="xxx" value="xxxx"/>
    </bean>
</beans>
```



## 自定义xml和xsd

可以参考 Jackson 的xml读取成 Java POJO 类。

### xsd

```xml
<?xml version="1.0" encoding= "UTF-8"?>
<xsd:schema xmlns= "http://customer/xsd-a" xmlns:xsd= "http://www.w3.org/2001/XMLSchema"
     targetNamespace="http://customer/xsd-a" elementFormDefault="qualified" >
     <xsd:simpleType name ="IDType"> 
           <xsd:restriction base ="xsd:string">
               <xsd:pattern value= "[\w,\-,_]{1,32}"></xsd:pattern >
           </xsd:restriction >
     </xsd:simpleType >
     <!-- 定义类型 -->
     <xsd:complexType name ="jobType">
           <xsd:sequence >
               <xsd:element name ="jobId" type="IDType">
                    <xsd:annotation >
                         <xsd:documentation ><![CDATA[任务ID，唯一区别一个并行计算任务，必须唯一 ]]></xsd:documentation >
                    </xsd:annotation >
               </xsd:element >
               <xsd:element name ="jobName" type= "xsd:string" minOccurs ="0">
                    <xsd:annotation >
                         <xsd:documentation ><![CDATA[任务名称，可选 ]]></xsd:documentation >
                    </xsd:annotation >
               </xsd:element >
             </xsd:sequence >
     </xsd:complexType >
     <!-- 定义类型 -->
     <xsd:complexType name ="jobBundleType">
           <xsd:sequence >
               <xsd:element name ="job" maxOccurs= "unbounded" type="jobType" ></xsd:element >
           </xsd:sequence >
           <xsd:attribute name ="bundleName" type= "IDType"></xsd:attribute >
     </xsd:complexType >
	<xsd:element name ="jobBundle" type="jobBundleType"> </xsd:element >
</xsd:schema>
```

### xml

```xml
<?xml version="1.0" encoding= "UTF-8"?>
<jobBundle bundleName= "sldfjlaksd" xmlns= "http://customer/xsd-a"
     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
     xsi:schemaLocation="http://customer/xsd-a  http://customer/xsd-a.xsd">
     <job>
           <jobId>df-asd_asdf238-sdf_adf</jobId>
           <distributorClassName ></distributorClassName >
     </job>
</jobBundle>

```

### java

```java
SAXReader reader = new SAXReader(true);
reader.setEntityResolver(new FileResolver());
reader.setFeature("http://apache.org/xml/features/validation/schema", true);
Document document = reader.read(new File("D:\\Project\\xmlparser\\src\\test.xml"));
Element root = document.getRootElement();

// FileResolver.java（读取自定义文件，将xml中自定义的xsd映射为本地文件读取，参考Spring的PluggableSchemaResolver）
public class FileResolver implements EntityResolver {
	@Override
	public InputSource resolveEntity(String publicId, String systemId) {
	    if (systemId != null && systemId.endsWith(".xsd")) {
	        String resourceLocation = getSchemaMappings().get(systemId);
	        if (resourceLocation != null) {
	            InputStream resource = FileResolver.class.getResourceAsStream(resourceLocation) ;
	            if (resource != null) {
	                InputSource source = new InputSource(resource);
	                source.setPublicId(publicId);
	                source.setSystemId(systemId);
	                return source;
	            }
	        }
	    }
	    return null;
	}
}
```

