# 整体流程

## 制品库

### [Nexus](./nexus.md)

- NPM；
- MVN；
- PYPI；
- GO；
- DOCKER；
- HELM；
- RAW：可用于前端打包出来的静态文件；



## CI/CD

Github + Github actions；

Github + travis 持续集成工具； Softagram （pull request reviews and code analysis)



需求描述：CI / CD + GitOps + ChatOps

- 通过PR的形式触发Preview，执行一系列的检查，并显示结果；
- 支持多套环境（开发/测试/生产/演示等）的部署；



### [Gitlab + Gitlab-ci](./gitlab.md)

持续集成工具；



### [JenkinsX](./jenkinsx.md)

- K8s CI/CD；

- 原生支持预览环境和 gitops；

  

### [Drone](./drone.md)

> Drone is a self-service **Continuous Integration** platform for busy development teams.

- 支持GitHub, GitHubEnterprise, Bitbucket, and GitLab；
- 使用Yaml作为配置；



### [ArgoCD](https://github.com/argoproj/argo-cd)

> Argo CD is a declarative, GitOps **continuous delivery** tool for Kubernetes.



### Rancher Pipeline



## ChatOps

### Prow

### [JenkinsX Lighthouse](./jenkinsx.md#jenkins-x Lighthouse)



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