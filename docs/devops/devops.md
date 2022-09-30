# DevOps

> Dev + Ops：开发及运维

背景：**敏捷开发为代表的持续开发模式**

- 迭代式的 开发 -> 测试 -> 交付 -> 运维；

基础设施及代码：代码可以将基础设置的配置代码化

运维部门的任务不是确保系统稳定、快速地运行，而是**确保商业的有效性**（开发部门也一样）；

- 尊重（Respect）
- 信任（Trust）
- 正确认识失败（Healthy attitude about Failure）
- 避免指责（Avoiding Blame）

DevOps优势：

- 消除对个人的依赖；
- 降低团队间的损耗；
- 提高质量：开发和运维团队，共享发布的时机和内容，掌握发布影响的范围，一起对服务进行监控；



## 实践

### 团队环境的准备

#### 虚拟环境构建工具

Vagrant：基于Virtualbox的虚拟机，用于管理虚拟机生命周期的命令行实用程序

Vagrant是一个基于Ruby的工具，用于创建和部署虚拟化开发环境。它 使用Oracle的开源[VirtualBox](https://baike.baidu.com/item/VirtualBox)虚拟化系统，使用 Chef创建自动化虚拟环境。

- 建立和删除虚拟机
- 配置虚拟机运行参数
- 管理虚拟机运行状态
- 自动配置和安装开发环境
- 打包和分发虚拟机运行环境



docker：轻量级容器技术

问题：

- 不容易理解构建步骤：shell脚本形式描述，不同人编写方式差别大，代码化不统一；
- 不能添加新的配置：难以对已创建的虚拟机添加新的配置项，强依赖shell脚本和条件语句；
- 构建步骤难以复用：不同环境可能如IP，JVM堆的参数不同，导致配置文件难以迁移和复用；



#### 构建工作通用化（基础设置配置管理工具）

[Ansible ](./ansible.md)或者`Chef`：声明式、抽象化、收敛性、幂等性、可移植性

- 消除环境构建步骤（代码）对个人的依赖；
  - 统一格式描述
- 在不同的环境中使用相同的步骤；
  - 环境变量定义，不同环境不同配置文件




#### 基础设施测试代码化

**Serverspec（基于ruby）**：能够**验证配置管理工具的配置结果**（Ansible、Puppet、Chef等），可以实现**基础设施测试代码化自动化**。

- 固定格式编写测试用例列表
- 输出测试结果报告（可以通过`coderay`的gem软件包实现 html 可视化结果）

[支持的服务类型和断言语句](https://serverspec.org/resource_types.html#service)

[Kirby, 最新2015](https://github.com/ks888/kirby)：对Serverspec计算Ansible代码覆盖率



## 项目代码开发

问题点：

- 各个成员代码规范不一致，冲突且不易懂；
- 代码提交冲突，代码任意提交；



解决：

- 项目管理POM，依赖冲突、编译、打包等方案；
- 插件统一代码、代码规范、文档规范风格；
- code review + merge request流程化 ；
- 自动化测试和部署，单元自动测试和测试环境；


不同组件之间的交互定义

- 前后端：需要定义接口文档（swagger）

- 服务间：

  - RPC（thrift， grpc），序列化采用protobuf

  - 微服务间：上层使用的json的http

    

Scala

- scalastyle_config.xml配置；（[常用的scalastyle配置](scala/scala_style.md)）
- scalafix：scala代码的规则问题检查和修复
- [scalafmt]( https://scalameta.org/scalafmt/ )：代码格式化工具，需要注意不要和scalastyle冲突
- scalatest单元测试；
