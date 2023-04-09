# STOMP

> Streaming Text Orientated Message Protocol，是流文本定向消息协议，是一种为MOM(Message Oriented Middleware，面向消息的中间件)设计的简单文本协议。
>
> STOMP is a subprotocol operating on top of the lower-level WebSocket.

提供消息体的格式，允许`STOMP客户端(Endpoints)`与任意`STOMP消息代理（message broker）`进行交互，实现客户端之间进行异步消息传送。

## 格式

STOMP的客户端和服务器之间的通信是通过“帧”（Frame）实现的，每个帧由多“行”（Line）组成。

- 第一行包含了命令，然后紧跟键值对形式的Header内容；
- 第二行必须是空行；
- 第三行开始就是Body内容，末尾都以空字符结尾。
  - 只有SEND、MESSAGE和ERROR帧可以有正文



## STOMP broker



## 示例

[Spring官方示例-前后端一体](https://spring.io/guides/gs/messaging-stomp-websocket/)

