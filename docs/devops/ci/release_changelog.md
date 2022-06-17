# 版本发布

## 0. 前提

参考 [Git Commit 规范](./commit_format.md)



## 1.  自动生成changelog并发布

### 1.1 自动生成changelog

利用[conventional-changelog](https://github.com/conventional-changelog/conventional-changelog)工具产生 changelog

```
npm install -g conventional-changelog-cli
```

如果之前每次 commit 都使用规范化的 commit 提交，那么使用:

```
conventional-changelog -i CHANGELOG.md -s -p angular 
```

### 1.2 自动生成changelog并发布（不采用）

> 对 CI 的适配不如 semantic-release 好

更上层的工具[standard-version](https://github.com/conventional-changelog/standard-version)来产生CHANGELOG.md，可以自动帮助我们做以下几件事情:

- 升级元数据中的版本号（如package.json,composer.json等等）
- 使用 [conventional-changelog](https://github.com/conventional-changelog/conventional-changelog) 更新  *CHANGELOG.md*
- 提交 package.json (如果有) 和 CHANGELOG.md
- 给新版本打一个 tag


安装：`npm i -g standard-version`

使用：`standard-version`

- `--dry-run`: 强烈建议正式运行之前先加这个参数，打印一下要做的事情，并不真正执行
- `--first-release/-f`: 首次发布，加上这个参数代表不会升级版本号。仅第一次使用需要
- `--commit-all/-a`: 等效于git commit -a，建议自己决定要提交哪些文件，都add没问题之后再执行standard-version
- `--release-as/-r`: 手工指定下一个版本号(格式<major|minor|patch>)。这个参数可能会经常使用。standard-version默认规则是从major/minor/patch中最后一个非零的字段开始累加，如v1.0.0 -> v2.0.0, v1.1.0 -> v1.2.0, v1.1.1 -> v1.1.2，指定这个参数可以规避这个规则，从自定义的版本号开始重新按上述规则升级
- `--prerelease/-p`: 预发布版本，默认产生的 tag 像这样: v1.0.1-0。可以跟上一个可选的 tag id。如-p alpha，将产生这样的 tag: v1.0.1-alpha.0

## 2. semantic-release 自动发布

> 更适合在 CI 环境中运行，自带支持各种 git server 的认证支持，如 Github，Gitlab，Bitbucket 等等，此外，还支持插件，以便完成其他后续的流程步骤，比如自动生成 git tag 和 release note 之后再 push 回中央仓库，自动发布 npm 包等等。

大致的工作流如下:

- 提交到**特定的分支**触发 release 流程
- 验证 commit 信息，生成 `release note`，打 `git tag`
- 其他后续流程，如生成`CHANGELOG.md`，`npm publish`等等（通过插件完成）

**版本号更新的逻辑**：只有 **feat** 和 **fix** 提交才会触发版本升级

- 如果包含 feat 记录，版本由1.**0**.0升级到了1.**1**.0
- 只有 fix 记录，版本由1.1.**0**升级到了1.1.**1**
- feat 且 commit footer内有`BREAKING CHANGE:` 提交将会升级主版本号,版本由1.2.0升级到了2.0.0



### 使用

- 项目工程中添加`.releaserc`配置

```javascript
{
  "plugins": [
    "@semantic-release/commit-analyzer",
    "@semantic-release/release-notes-generator",
    "@semantic-release/changelog",
    "@semantic-release/git"
  ]
}
```

- `.gitlab-ci.yml` 配置(gitlab-runner运行环境为powershell)

```yaml
# lint 过程用于检测 commitlint 结果
# release 过程用于自动化产生 git tag 和 CHANGELOG.md

stages:
  - lint
  - release

commitlint:
  stage: lint
  image: node:lts
  script: 
    - npm install -g @commitlint/cli @commitlint/config-conventional;
    - if("${CI_COMMIT_BEFORE_SHA}" -eq "0000000000000000000000000000000000000000") { npx commitlint -x @commitlint/config-conventional -f HEAD^ }else{ npx commitlint -x @commitlint/config-conventional -f "${CI_COMMIT_BEFORE_SHA}"}


release:
  stage: release
  image: node:lts
  script:
    - npm install -D semantic-release @semantic-release/gitlab @semantic-release/changelog conventional-changelog-conventionalcommits @semantic-release/git
    - npx semantic-release
  # 仅在中央仓库的分支发生提交时才触发 release 流程
  only:
    - master
```

- gitlab项目**环境变量配置**（GITLAB_TOKEN或者GL_TOKEN）



效果图：

![CHANGELOG出现在中央仓库](pics/changelog.png)

### 插件

- [@semantic-release/git ](https://github.com/semantic-release/git) 插件，还会将生成的`CHANGELOG.md` 反向 push 回中央仓库

- 

修改源码部分的版本号配置内容