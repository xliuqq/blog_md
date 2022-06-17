[toc]

# Guice
轻量级IoC（依赖注入框架）容器，类似Spring-AOP。Guice 通过代码的形式来注入并管理依赖。

- Guice：整个框架的门面
- Injector：一个依赖的管理上下文
- Binder：一个接口和实现的绑定
- Module：一组 Binder
- Provider：bean 的提供者
- Key：Binder 中对应一个 Provider
- Scope：Provider 的作用域