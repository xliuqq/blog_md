# SBT 源

**问题1：IDEA 中 SBT 下载非常慢 !**

查看~/.sbt/boot/update.log的日志



**问题：IDEA新建工程后dump project structure from sbt 非常慢**

配置 sbt 的阿里源，然后查看日志，是否从阿里源上进行下载



**配置**

linux在 ~/.sbt 目录下；windows 在$user/.sbt 目录下

新建文件名 repositories

写入下面内容

```ini
[repositories]
local
public: http://maven.aliyun.com/nexus/content/groups/public/
typesafe:http://dl.bintray.com/typesafe/ivy-releases/ , [organization]/[module]/(scala_[scalaVersion]/)(sbt_[sbtVersion]/)[revision]/[type]s/[artifact](-[classifier]).[ext], bootOnly
ivy-sbt-plugin:http://dl.bintray.com/sbt/sbt-plugin-releases/, [organization]/[module]/(scala_[scalaVersion]/)(sbt_[sbtVersion]/)[revision]/[type]s/[artifact](-[classifier]).[ext]
sonatype-oss-releases
sonatype-oss-snapshots
```

