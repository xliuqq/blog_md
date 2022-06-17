# SpringCloudGateway

 Spring Cloud Gateway 基于 [Spring Boot 2.x](https://spring.io/projects/spring-boot#learn), [Spring WebFlux](https://docs.spring.io/spring/docs/current/spring-framework-reference/web-reactive.html), and [Project Reactor](https://projectreactor.io/docs) 。

因此一些同步库（Spring Data and Spring Security等）无法一起使用。

**依赖运行时的Netty。**

## Eureka 自动服务发现配置

**如果`Eureka Server`上有两个相同`serviceId`的服务时，`SpringCloud Gateway`会自动完成负载均衡。**

```yaml
# 服务名称
spring:
  application:
    name: spring-cloud-gateway
  # 开启 Gateway 服务注册中心服务发现
  cloud:
    gateway:
      discovery:
        locator:
          enabled: true
# Eureka Server 配置
eureka:
  client:
    service-url:
      defaultZone: http://localhost:10000/eureka/
# 配置Gateway日志等级，输出转发细节信息
logging:
  level:
    org.springframework.cloud.gateway: debug
```

`spring.cloud.gateway.discovery.locator.enabled`设置为true时，将 `/serviceId/**`匹配到对应的服务；并且将 `/serviceId/(?<remaining>.*)`URL重写为 `/${remaining}`。



## 过滤

gateway 只有俩种过滤器：“pre” 和 “post”。

- **PRE**： 这种过滤器在请求被路由之前调用。
- **POST**：这种过滤器在路由到微服务以后执行。

GatewayFilter 与 GlobalFilter。

- **GlobalFilter**： 全局过滤器
- **GatewayFilter**： 将应用到单个路由或者一个分组的路由上。