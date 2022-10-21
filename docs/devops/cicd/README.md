# CI/CD

Github + Github actions；

Github + travis 持续集成工具； Softagram （pull request reviews and code analysis)



## [Gitlab + Gitlab-ci](./gitlab.md)

持续集成工具；



## [Jenkins](http://jenkins-ci.org/)

- 可以用于CI（持续集成），支持多种语言，多种代码检查插件等；

- 持续、自动构建/测试软件项目，监控一些定时任务；

  



## 代码审查

### SonarLint



### SonarQube

安装：https://docs.sonarqube.org/latest/setup/get-started-2-minutes/

- 需要JDK11，且以非root用户启动

与 Jenkins 集成

https://www.cnblogs.com/cjsblog/p/10740840.html

与 Jacoco 集成

```properties
sonar.java.coveragePlugin=jacoco
# 确定xml文件的名称
sonar.coverage.jacoco.xmlReportPaths=target/site/jacoco/jacoco.xml
sonar.junit.reportsPath=target/surefire-reports
```

### CheckStyle



### FindBugs



### [codecov](https://docs.codecov.io/docs/supported-languages)

- 支持多种CI工具如 travis、gitlab-ci、Jenkins，支持多种语言，进行代码覆盖率；





## github徽章

https://lpd-ios.github.io/2017/05/03/GitHub-Badge-Introduction/