# JMH

http://openjdk.java.net/projects/code-tools/jmh/

运行 JMH 基准测试的**推荐方法**是使用Maven设置一个独立的项目，该项目依赖于应用程序的jar文件。

```shell
$ mvn archetype:generate \
          -DinteractiveMode=false \
          -DarchetypeGroupId=org.openjdk.jmh \
          -DarchetypeArtifactId=jmh-java-benchmark-archetype \
          -DgroupId=org.sample \
          -DartifactId=test \
          -Dversion=1.0
          
$ cd test/
$ mvn clean install

java -jar target/benchmarks.jar
```

> JMH是Open JDK9自带的，如果你是 JDK9 之前的版本也可以通过导入 openjdk

```xml
<dependency>
    <groupId>org.openjdk.jmh</groupId>
    <artifactId>jmh-core</artifactId>
    <version>1.19</version>
</dependency>
<dependency>
    <groupId>org.openjdk.jmh</groupId>
    <artifactId>jmh-generator-annprocess</artifactId>
    <version>1.19</version>
</dependency>
```

FileSearchBenchmark.java 