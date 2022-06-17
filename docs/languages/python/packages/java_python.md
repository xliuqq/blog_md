# Java/Python互相调用

## [JPype](https://github.com/jpype-project/jpype)

> JPype is cross language bridge to allow Python programs full access to Java class libraries.



## [PyJnius](https://github.com/kivy/pyjnius)

A Python module to access Java classes as Python classes using the Java Native Interface (JNI)

### 入门

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

## [Py4j](https://www.py4j.org/)

> Py4J enables Python programs to dynamically access arbitrary Java objects, also enables Java programs to call back Python objects.

Python for Java，通过 **socket** 进行消息发送，使得 Python 中可以调用 Java代码 / Java 也可以回调Python对象。

### 入门

Java 端启动服务：

```java
GatewayServer gatewayServer = new GatewayServer(new EntryPoint(), 25555);
gatewayServer.start();
System.out.println("Gateway Server Started");
```

Python端：

```python
from py4j.java_gateway import JavaGateway, GatewayParameters

gateway = JavaGateway(gateway_parameters=GatewayParameters(port=25555))
# 获取String类型
JString = gateway.jvm.java.lang.String

# 转换成Python中的字符串
a = JString("213")
```

TODO: Java 回调 Python