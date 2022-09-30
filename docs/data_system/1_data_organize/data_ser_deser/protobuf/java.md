# Java ProtoBuf / GRPC

## maven配置

可以参考github上`grpc-java`项目中example中的示例pom.xml

https://github.com/grpc/grpc-java/blob/master/README.md

```xml
 <properties>
    <grpc.version>1.17.1</grpc.version>
    <protobuf.version>3.5.1</protobuf.version>
    <protoc.version>3.5.1</protoc.version>
  </properties>

  <dependencies>
<!--             <dependency>-->
<!--            <groupId>io.grpc</groupId>-->
<!--            <artifactId>grpc-netty-shaded</artifactId>-->
<!--            <scope>runtime</scope>-->
<!--            <version>${grpc.version}</version>-->
<!--        </dependency>-->
    <dependency>
      <groupId>io.grpc</groupId>
      <artifactId>grpc-protobuf</artifactId>
      <version>${grpc.version}</version>
    </dependency>
    <dependency>
      <groupId>io.grpc</groupId>
      <artifactId>grpc-stub</artifactId>
      <version>${grpc.version}</version>
    </dependency>

    <dependency>
      <groupId>com.google.protobuf</groupId>
      <artifactId>protobuf-java</artifactId>
      <version>${protobuf.version}</version>
    </dependency>

  </dependencies>


  <build>
     <!-- 必不可少，用于发现os相关的配置 -->
    <extensions>
      <extension>
        <groupId>kr.motd.maven</groupId>
        <artifactId>os-maven-plugin</artifactId>
        <version>1.6.2</version>
      </extension>
    </extensions>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-compiler-plugin</artifactId>
        <version>3.2</version>
        <configuration>
          <source>1.8</source>
          <target>1.8</target>
          <encoding>UTF-8</encoding>
          <maxmem>1024m</maxmem>
          <compilerArgs>
            <arg>-Xlint:none</arg>
          </compilerArgs>
        </configuration>
      </plugin>
      <plugin>
        <!--  refer to https://www.xolstice.org/protobuf-maven-plugin/ -->
        <groupId>org.xolstice.maven.plugins</groupId>
        <artifactId>protobuf-maven-plugin</artifactId>
        <version>0.6.1</version>
        <configuration>
          <!--默认值-->
          <protoSourceRoot>${project.basedir}/src/main/proto</protoSourceRoot>
          <outputDirectory>${project.build.sourceDirectory}</outputDirectory>
          <!--设置是否在生成java文件之前清空outputDirectory的文件，默认值为true，设置为false时也会覆盖同名文件-->
          <clearOutputDirectory>false</clearOutputDirectory>
          <!--默认值-->
          <temporaryProtoFileDirectory>${project.build.directory}/generated-sources/protobuf/java
          </temporaryProtoFileDirectory>
          <!--更多配置信息可以查看https://www.xolstice.org/protobuf-maven-plugin/compile-mojo.html-->
          <protocArtifact>com.google.protobuf:protoc:${protoc.version}:exe:${os.detected.classifier}
          </protocArtifact>
          <pluginArtifact>io.grpc:protoc-gen-grpc-java:${grpc.version}:exe:${os.detected.classifier}
          </pluginArtifact>
          <pluginId>grpc-java</pluginId>
        </configuration>
        <executions>
          <execution>
            <goals>
              <goal>compile</goal>
              <goal>compile-custom</goal>
            </goals>
          </execution>
        </executions>
      </plugin>
    </plugins>
  </build>
```

