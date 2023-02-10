# 基于Gitlab的Devops实战

## 源码

[Gitlab仓库部署](../codehub/gitlab.md)：采用 Gitlab 进行源码管理



## 开发分支模式

分支模式采用 [master/dev/feature](./git_branch_model.md) 分支模型

- master：发布版本，保证生产可用；
- dev：功能版本（快照版本），当前版本的功能集成；
- feature：特性版本，当前版本的特性功能开发；



## Commit规范和版本发布

[版本自动发布](./cicd/release_changelog.md) ：采用规范的 commit 信息

- master 分支出 release 版本形成 tag，从 dev 提交 PR 合并；
- dev 分支做开发，从 feature 分支提交 PR 合并，出 snapshot 版本，不发布版本；
- dev 测试完成后，合入 master，后续版本开发在发布版本前不合入 dev；
- master 出版本后，会生成版本commit记录，合并回 dev；



## CI

[持续集成CI](./cicd/gitlab-ci.md)：采用 Gitlab CI 做持续集成

- 集成环境独立于开发/测试/生产环境；
- 可以通过 docker runner / k8s runner；



**自动构建**：

- <font color=red>*feature 分支：自动触发构建，发布 SNAPSHOT 版本 ？*</font>

- dev 分支 ：自动触发构建，发布 SNAPSHOT 版本；

  - 测试人员更希望手动触发版本？

- master：自动触发构建，发布 RELEASE 版本；

  - 同 DEV 分支进行构建，但版本为 RELEASE 的版本； 

  - 仅对 master 分支，执行`semantic-release`生成版本CHANGELOG信息；

    

手动构建：

- 选择 feature 触发构建，发布 SNAPSHOT 版本，在 开发 环境验证；
- 选择 dev 触发构建，发布 SNAPSHOT 版本，在 测试 环境验证；
- 选择 master 触发构建，发布 RELEASE 版本，部署 生产 环境；



## CD

自动部署：argocd 采用监听 git 仓库**特定分支的特定目录下的Yaml**的形式，自动进行部署。

采用 [master/dev/feature](./git_branch_model.md) 分支模型 进行开发：

- 开发环境：监听特性分支的变动；
  - <font color=red>argocd 监听的分支名不支持正则，如何处理？；</font>
  - 开发环境主动推送？
- 测试环境：监听 dev 分支的 yaml 变动； 
  - <font color=red> dev 分支并不支持 Git 变更回推 </font>

- 生产环境：监听 master 分支的 yaml 变动；



手动部署：

- 选择手工构建完成后的相应版本，部署到环境；



## Issue Tracker

[问题跟踪](./cicd/gitlab-jira.md)：采用Gitlab 集成 Jira 进行问题跟踪（创建，修改，关闭等）

- 使用 dev 作为默认分支

## Artifacts

[制品库](./cicd/nexus.md)：采用 nexus 作制品库（maven/npm/image 等）

- 可选：采用 habor 作为 docker image 库

## Notifier

事件通知：钉钉集成

- CI 通知，Gitlab Webhook 配置 Pipeline 结果；（钉钉的极弧Gitlab不支持）
- CD 通知，Argo Workflow ？



## Chatops：TODO

- Tekton Pipeline：https://github.com/jenkins-x/lighthouse/blob/main/docs/install_lighthouse_with_tekton.md
- Jenkins Job：https://github.com/jenkins-x/lighthouse/blob/main/docs/install_lighthouse_with_jenkins.md



