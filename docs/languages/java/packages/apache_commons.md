[toc]

# Apache Commons

> 2021/10



## 常用的

### Collections

ArrayUtils, StringUtils 等工具类

### Configuration

properties配置高级读取方法（读取成List或者Array等）：

```java
FileBasedConfigurationBuilder<PropertiesConfiguration> builder = new FileBasedConfigurationBuilder<>(PropertiesConfiguration.class).configure(new Parameters().properties().setFileName("conf/conf.properties").setListDelimiterHandler(new DefaultListDelimiterHandler(',')));
```

### IO

FileUtils：deleteDirectory，contentEquals，contentEqualsIgnoreEOL，isFileNewer，isFileOlder，copyFileToDirectory

### Lang

Pair, Triple类



## Proper

| Component                                                    | Description                                                  | Release Date   |
| :----------------------------------------------------------- | :----------------------------------------------------------- | :------------- |
| [BCEL](https://commons.apache.org/proper/commons-bcel/)      | Byte Code Engineering Library - analyze, create, and manipulate Java class files | 2020-06-10     |
| [BeanUtils](https://commons.apache.org/proper/commons-beanutils/) | Easy-to-use wrappers around the Java reflection and introspection APIs. | 2019-08-13     |
| [BSF](https://commons.apache.org/proper/commons-bsf/)        | Bean Scripting Framework - interface to scripting languages, including JSR-223 | 2010-06-24     |
| ~~[Chain](https://commons.apache.org/proper/commons-chain/)~~ | ~~*Chain of Responsibility* pattern implemention.~~          | 2010 -04-12    |
| [CLI](https://commons.apache.org/proper/commons-cli/)        | Command Line arguments parser.                               | 2017-03-09     |
| [Codec](https://commons.apache.org/proper/commons-codec/)    | General encoding/decoding algorithms (for example phonetic, base64, URL). | 2020-09-01     |
| [Collections](https://commons.apache.org/proper/commons-collections/) | Extends or augments the Java Collections Framework.          | 2019-07-08     |
| [Compress](https://commons.apache.org/proper/commons-compress/) | Defines an API for working with tar, zip and bzip2 files.    | 2020-02-08     |
| [Configuration](https://commons.apache.org/proper/commons-configuration/) | Reading of configuration/preferences files in various formats. | 2020-03-11     |
| [Crypto](https://commons.apache.org/proper/commons-crypto/)  | A cryptographic library optimized with AES-NI wrapping Openssl or JCE algorithm implementations. | 2020-09-02     |
| [CSV](https://commons.apache.org/proper/commons-csv/)        | Component for reading and writing comma separated value files. | 2020-02-06     |
| [Daemon](https://commons.apache.org/proper/commons-daemon/)  | Alternative invocation mechanism for unix-daemon-like java code. | 2019-09-30     |
| [DBCP](https://commons.apache.org/proper/commons-dbcp/)      | Database connection pooling services.                        | 2020-09-25     |
| [DbUtils](https://commons.apache.org/proper/commons-dbutils/) | JDBC helper library.                                         | 2017-07-20     |
| [Digester](https://commons.apache.org/proper/commons-digester/) | XML-to-Java-object mapping utility.                          | 2011-12-13     |
| [Email](https://commons.apache.org/proper/commons-email/)    | Library for sending e-mail from Java.                        | 2017-08-01     |
| ~~[Exec](https://commons.apache.org/proper/commons-exec/)~~  | ~~API for dealing with external process execution and environment management in Java.~~ | 2014-11-06     |
| [FileUpload](https://commons.apache.org/proper/commons-fileupload/) | File upload capability for your servlets and web applications. | 2019-01-16     |
| ~~[Functor](https://commons.apache.org/proper/commons-functor/)~~ | ~~A functor is a function that can be manipulated as an object, or an object representing a single, generic function.~~ | 2014-02-17     |
| [Geometry](https://commons.apache.org/proper/commons-geometry/) | Space and coordinates.                                       | 2020-07-19     |
| [Imaging (previously called Sanselan)](https://commons.apache.org/proper/commons-imaging/) | A pure-Java image library.                                   | 2019-05-02     |
| [IO](https://commons.apache.org/proper/commons-io/)          | Collection of I/O utilities.                                 | 2020-09-10     |
| ~~[JCI](https://commons.apache.org/proper/commons-jci/)~~    | ~~Java Compiler Interface~~                                  | 2013-10-14     |
| [JCS](https://commons.apache.org/proper/commons-jcs/)        | Java Caching System                                          | 2020-08-16     |
| [Jelly](https://commons.apache.org/proper/commons-jelly/)    | XML based scripting and processing engine.                   | 2017-09-27     |
| **[Jexl](https://commons.apache.org/proper/commons-jexl/)**  | **Expression language which extends the Expression Language of the JSTL.** | **2017-04-14** |
| [JXPath](https://commons.apache.org/proper/commons-jxpath/)  | Utilities for manipulating Java Beans using the XPath syntax. | 2008-08-14     |
| [Lang](https://commons.apache.org/proper/commons-lang/)      | Provides extra functionality for classes in java.lang.       | 2021-03-01     |
| [Logging](https://commons.apache.org/proper/commons-logging/) | Wrapper around a variety of logging API implementations.     | 2014-07-11     |
| [Math](https://commons.apache.org/proper/commons-math/)      | Lightweight, self-contained mathematics and statistics components. | 2015-04-17     |
| [Net](https://commons.apache.org/proper/commons-net/)        | Collection of network utilities and protocol implementations. | 2021-02-19     |
| [Numbers](https://commons.apache.org/proper/commons-numbers/) | Number types (complex, quaternion, fraction) and utilities (arrays, combinatorics). | 2020-04-08     |
| [OGNL](https://commons.apache.org/proper/commons-ognl/)      | An Object-Graph Navigation Language                          | 2013-??-??     |
| [Pool](https://commons.apache.org/proper/commons-pool/)      | Generic object pooling component.                            | 2020-09-29     |
| [Proxy](https://commons.apache.org/proper/commons-proxy/)    | Library for creating dynamic proxies.                        | 2008-02-28     |
| [RDF](https://commons.apache.org/proper/commons-rdf/)        | Common implementation of RDF 1.1 that could be implemented by systems on the JVM. | 2017-12-07     |
| [RNG](https://commons.apache.org/proper/commons-rng/)        | Implementations of random numbers generators.                | 2019-11-11     |
| [SCXML](https://commons.apache.org/proper/commons-scxml/)    | An implementation of the State Chart XML specification aimed at creating and maintaining a Java SCXML engine. It is capable of executing a state machine defined using a SCXML document, and abstracts out the environment interfaces. | 2008-12-01     |
| [Statistics](https://commons.apache.org/proper/commons-statistics/) | Statistics.                                                  | ????-??-??     |
| [Text](https://commons.apache.org/proper/commons-text/)      | Apache Commons Text is a library focused on algorithms working on strings. | 2019-07-25     |
| [Validator](https://commons.apache.org/proper/commons-validator/) | Framework to define validators and validation rules in an xml file. | 2020-08-07     |
| [VFS](https://commons.apache.org/proper/commons-vfs/)        | Virtual File System component for treating files, FTP, SMB, ZIP and such like as a single logical file system. | 2021-03-10     |
| [Weaver](https://commons.apache.org/proper/commons-weaver/)  | Provides an easy way to enhance (weave) compiled bytecode.   | 2018-09-07     |



## Sandbox（正在开发的项目）

| 模块                                                         | 功能                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [BeanUtils2](https://commons.apache.org/sandbox/commons-beanutils2/) | Redesign of Commons BeanUtils.                               |
| [ClassScan](https://commons.apache.org/sandbox/commons-classscan/) | Find Class interfaces, methods, fields, and annotations without loading. |
| [CLI2](https://commons.apache.org/sandbox/commons-cli2)      | Redesign of Commons CLI.                                     |
| [Convert](https://commons.apache.org/sandbox/commons-convert/) | Commons-Convert aims to provide a single library dedicated to the task of converting an object of one type to another. |
| [Finder](https://commons.apache.org/sandbox/commons-finder/) | Java library inspired by the UNIX find command.              |
| [Flatfile](https://commons.apache.org/sandbox/commons-flatfile/) | Java library for working with flat data structures.          |
| [Graph](https://commons.apache.org/sandbox/commons-graph/)   | A general purpose Graph APIs and algorithms.                 |
| [I18n](https://commons.apache.org/sandbox/commons-i18n/)     | Adds the feature of localized message bundles that consist of one or many localized texts that belong together. Think of an error message that consists of title, text, summary and error details. |
| [Id](https://commons.apache.org/sandbox/commons-id/)         | Id is a component used to generate identifiers.              |
| [Inject](https://commons.apache.org/sandbox/commons-inject/) | Implementation of JSR 330, a standards compliant dependency injection framework |
| [Javaflow](https://commons.apache.org/sandbox/commons-javaflow/) | Continuation implementation to capture the state of the application. |
| [JNet](https://commons.apache.org/sandbox/commons-jnet/)     | JNet allows to use dynamically register url stream handlers through the java.net API. |
| [Monitoring](https://commons.apache.org/sandbox/commons-monitoring/) | Monitoring aims to provide a simple but extensible monitoring solution for Java applications. |
| [Nabla](https://commons.apache.org/sandbox/commons-nabla/)   | Nabla provides automatic differentiation classes that can generate derivative of any function implemented in the Java language. |
| [OpenPGP](https://commons.apache.org/sandbox/commons-openpgp/) | Interface to signing and verifying data using OpenPGP.       |
| [Performance](https://commons.apache.org/sandbox/commons-performance/) | A small framework for microbenchmark clients, with implementations for Commons DBCP and Pool. |
| [Pipeline](https://commons.apache.org/sandbox/commons-pipeline/) | Provides a set of pipeline utilities designed around work queues that run in parallel to sequentially process data objects data objects. |



## Dormant（刚启动或者已经停止维护的项目）

| 模块                                                         | 描述                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [Attributes](https://commons.apache.org/dormant/commons-attributes/) | Runtime API to metadata attributes such as doclet tags.      |
| [Betwixt](https://commons.apache.org/dormant/commons-betwixt/) | Services for mapping JavaBeans to XML documents, and vice versa. |
| [Cache](https://commons.apache.org/dormant/cache/)           | Cache provides object caching services.                      |
| [Clazz](https://commons.apache.org/dormant/clazz/)           | Clazz focuses on introspection and class manipulation.       |
| [Contract](https://commons.apache.org/dormant/contract/)     | This component makes all the nice features available to the java programming language that come along with contract based programming. |
| [Convert](https://commons.apache.org/dormant/convert/)       | Commons-Convert aims to provide a single library dedicated to the task of converting an object of one type to another. |
| [EL](https://commons.apache.org/dormant/commons-el/)         | Interpreter for the Expression Language defined by the JSP 2.0 specification. |
| [Discovery](https://commons.apache.org/dormant/discovery/)   | Tools for locating resources by mapping service/reference names to resource names. |
| [Events](https://commons.apache.org/dormant/events/)         | Commons-Events provides additional classes for firing and handling events. It focusses on the Java Collections Framework, providing decorators to other collections that fire events. |
| [Feedparser](https://commons.apache.org/dormant/feedparser/) | A Java RSS/Atom parser designed to elegantly support all versions of RSS (0.9, 0.91, 0.92, 1.0, and 2.0), Atom 0.5 (and future versions) as well as easy ad hoc extension and RSS 1.0 modules capability. |
| [JJar](https://commons.apache.org/dormant/jjar/)             | Jakarta JAR Archive Repository                               |
| [Launcher](https://commons.apache.org/dormant/launcher/)     | Cross platform Java application launcher.                    |
| [Latka](https://commons.apache.org/dormant/latka/)           | Commons-Latka is an HTTP functional testing suite for automated QA, acceptance and regression testing. |
| [Mapper](https://commons.apache.org/dormant/mapper/)         | Mapper is a thin abstraction layer around a project's chosen data mapping technology (a.k.a. DAO pattern). |
| [Messenger](https://commons.apache.org/dormant/messenger/)   | Messenger is an easy to use and lightweight framework for working with JMS in the web tier. |
| [Modeler](https://commons.apache.org/dormant/commons-modeler/) | Mechanisms to create Model MBeans compatible with JMX specification. |
| [Primitives](https://commons.apache.org/dormant/commons-primitives/) | Smaller, faster and easier to work with types supporting Java primitive types. |
| [Resources](https://commons.apache.org/dormant/resources/)   | Resources provides a lightweight framework for defining and looking up internationalized message strings keyed by a java.util.Locale and a message key. |
| [Scaffold](https://commons.apache.org/dormant/scaffold/)     | Scaffold is a toolkit for building web applications.         |
| [ThreadPool](https://commons.apache.org/dormant/threadpool/) | ThreadPool is a simple component for asynchronously dispatching work to another thread in a pool for simple multi threaded programming. |
| [Transaction](https://commons.apache.org/proper/commons-transaction/) | Implementations for multi level locks, transactional collections and transactional file access. |
| [Workflow](https://commons.apache.org/dormant/workflow/)     | Workflow provides a framework for building workflow management systems. |
| [XMLIO](https://commons.apache.org/dormant/xmlio/)           | Simple and fast importer for XML configuration or import files. |