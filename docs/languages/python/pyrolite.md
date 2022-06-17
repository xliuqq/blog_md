# Pyro

Pyro enables you to build applications in which objects can talk to each other over the network, with minimal programming effort.

## Pyrolite（Pyro客户端）

### Maven 

group id `net.razorvine` artifact id `pyrolite`。

### 序列化

[Pickle](https://github.com/irmen/pickle)

提供 Java 对象和Pickle对象的互相转换。

Note：4.x版本将Pickle序列化集成在代码中，5.x版本将pickle单独拆到独立项目中。

[Serpent](https://github.com/irmen/Serpent)



Java代码示例：

```java
import net.razorvine.pyro.*;

NameServerProxy ns = NameServerProxy.locateNS(null);
PyroProxy remoteobject = new PyroProxy(ns.lookup("Your.Pyro.Object"));
Object result = remoteobject.call("pythonmethod", 42, "hello", new int[]{1,2,3});
String message = (String)result;  // cast to the type that 'pythonmethod' returns
System.out.println("result message="+message);
remoteobject.close();
ns.close();
```