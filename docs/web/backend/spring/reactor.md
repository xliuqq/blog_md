# Reactor

> Reactive（反应式编程）：观察者模式，“推” 机制；
>
> Imperative（指令式编程）：迭代器模式，“拉”机制；

两个主要的模块：reactor-core 和 reactor-ipc。前者主要负责 Reactive Programming 相关的核心 API 的实现，后者负责高性能网络通信的实现，目前是基于 Netty 实现的。

## 概念

> **Reactive Streams 是规范，Reactor 实现了 Reactive Streams。Web Flux 以 Reactor 为基础，实现 Web 领域的反应式编程框架。**

在 Java 平台上，Netflix（开发了 RxJava）、TypeSafe（开发了 Scala、Akka）、Pivatol（开发了 Spring、Reactor）共同制定了一个被称为 [Reactive Streams 项目（规范）](https://github.com/reactive-streams/reactive-streams-jvm)，用于制定反应式编程相关的规范以及接口。其主要的接口有这三个：

- `Publisher`
- `Subscriber`
- `Subcription`
- `Processor`

其中，`Subcriber` 中包含 `onNext`、`onError`、`onCompleted` 这三个方法。

- 如果 Publisher 发布消息太快了，超过了 Subscriber 的处理速度， Backpressure 控制消费消息的速度；
- `Scheduler` 表示背后驱动反应式流的调度器，通常由各种线程池实现

## 使用

`Mono` 实现了 `org.reactivestreams.Publisher` 接口，代表**0到1个元素**的发布者。

`Flux` 同样实现了 `org.reactivestreams.Publisher` 接口，代表**0到N个元素**的发表者。



### 创建

`just`、`fromXXX`、`create`。

```java
Mono<String> helloWorld = Mono.just("Hello World");
Flux<String> fewWords = Flux.just("Hello", "World");
Flux<String> manyWords = Flux.fromIterable(words);
```



### 转换

中间阶段的 Mono 和 Flux 的方法主要有 `filter`、`map`、`flatMap`、`then`、`zip`、`reduce` 等

- `zip`：并发执行，如对两个 Mono 的执行并发，并做处理；



### 分支

`switchIfEmpty`：Fallback to an alternative Mono if this mono is completed without data。

使用 flatmap 和 then 模拟 **顺序+分支** 代码；

```java
status
    .flatMap(s -> {
        System.out.println("handle status");
        if (s != 0) {
            return Mono.error(new RuntimeException("status is not illegal"));
        }

        return Mono.empty();
    })
    .then(Mono.just(0)
            .flatMap(s -> {
                System.out.println("handle right");
                if (s != 0) {
                    return Mono.error(new RuntimeException("token is expired"));
                }

                return Mono.empty();
            }));
```



### 结束

直接消费的 Mono 或 Flux 的方式就是调用 `subscribe` 方法。