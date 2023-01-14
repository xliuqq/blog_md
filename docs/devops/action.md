# 基于Gitlab的Devops实战

[Gitlab仓库部署](../codehub/gitlab.md)：采用 Gitlab 进行源码管理

[持续集成CI](./cicd/gitlab-ci.md)：采用 Gitlab CI 做持续集成

[版本自动发布](./cicd/release_changelog.md) ：采用规范的 commit 信息

- master 分支出 release 版本形成 tag，从 dev 提交 PR 合并；
- dev 分支做开发，从 feature 分支提交 PR 合并，出 snapshot 版本，不发布版本；

[问题跟踪](./cicd/gitlab-jira.md)：采用Gitlab 集成 Jira 进行问题跟踪（创建，修改，关闭等）

- 使用 dev 作为默认分支

[制品库](./cicd/nexus.md)：采用 nexus 作制品库（maven/npm/image 等）

- 可选：采用 habor 作为 docker image 库

事件通知：钉钉集成

- CI 通知，Gitlab Webhook 配置 Pipeline 结果；（钉钉的极弧Gitlab不支持）
- CD 通知，Argo Workflow ？



Chatops：TODO

- Tekton Pipeline：https://github.com/jenkins-x/lighthouse/blob/main/docs/install_lighthouse_with_tekton.md
- Jenkins Job：https://github.com/jenkins-x/lighthouse/blob/main/docs/install_lighthouse_with_jenkins.md