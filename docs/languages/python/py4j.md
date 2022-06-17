[toc]

# JPype



# PyJnius

A Python module to access Java classes as Python classes using the Java Native Interface (JNI)

## 入门

```shell
pip install pyjnius
```

```python
>>> from jnius import autoclass
>>> autoclass('java.lang.System').out.println('Hello world')
Hello world

>>> Stack = autoclass('java.util.Stack')
>>> stack = Stack()
>>> stack.push('hello')
>>> stack.push('world')
>>> print stack.pop()
world
>>> print stack.pop()
hello
```

# Py4j

Python for Java，通过 socket 进行消息发送，使得 Python 中可以调用 Java代码。

TODO：

## 入门

```java
// java main 
GatewayServer gatewayServer = new GatewayServer(new EntryPoint(), 25555);
        gatewayServer.start();
        System.out.println("Gateway Server Started");
```



```python
from py4j.java_gateway import JavaGateway, GatewayParameters

gateway = JavaGateway(gateway_parameters=GatewayParameters(port=25555))
# 获取String类型
JString = gateway.jvm.java.lang.String

# 转换成Python中的字符串
a = JString("213")
```



思考：

1）Java中需要传入String.class，这种类定义的时候，怎么实现？

