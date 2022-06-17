## Pickle

链接：https://github.com/irmen/pickle

Apache Spark采用该库将Java对象序列化成Pickle格式，然后Python进行读取。



Java 读写 pickle （Python序列化格式）文件。

Pickle protocol version support: 

- reading: 0,1,2,3,4,5;
- writing: 2. 



### Python to Java (unpickling)

| PYTHON                                     | JAVA                                                         |
| ------------------------------------------ | ------------------------------------------------------------ |
| None                                       | null                                                         |
| bool                                       | boolean                                                      |
| int                                        | int                                                          |
| long                                       | long or BigInteger (depending on size)                       |
| string                                     | String                                                       |
| unicode                                    | String                                                       |
| complex                                    | net.razorvine.pickle.objects.ComplexNumber                   |
| datetime.date                              | java.util.Calendar                                           |
| datetime.datetime                          | java.util.Calendar                                           |
| datetime.time                              | net.razorvine.pickle.objects.Time                            |
| datetime.timedelta                         | net.razorvine.pickle.objects.TimeDelta                       |
| float                                      | double (float isn't used)                                    |
| array.array                                | array of appropriate primitive type (char, int, short, long, float, double) |
| list                                       | java.util.List                                               |
| tuple                                      | Object[]                                                     |
| set                                        | java.util.Set                                                |
| dict                                       | java.util.Map                                                |
| bytes                                      | byte[]                                                       |
| bytearray                                  | byte[]                                                       |
| decimal                                    | BigDecimal                                                   |
| custom class                               | Map<String, Object> (dict with class attributes including its name in "**class**") |
| Pyro4.core.URI                             | net.razorvine.pyro.PyroURI                                   |
| Pyro4.core.Proxy                           | net.razorvine.pyro.PyroProxy                                 |
| Pyro4.errors.*                             | net.razorvine.pyro.PyroException                             |
| Pyro4.utils.flame.FlameBuiltin             | net.razorvine.pyro.FlameBuiltin                              |
| Pyro4.utils.flame.FlameModule              | net.razorvine.pyro.FlameModule                               |
| Pyro4.utils.flame.RemoteInteractiveConsole | net.razorvine.pyro.FlameRemoteConsole                        |

### Java to Python (pickling)

| JAVA                         | PYTHON                                                       |
| ---------------------------- | ------------------------------------------------------------ |
| null                         | None                                                         |
| boolean                      | bool                                                         |
| byte                         | int                                                          |
| char                         | str/unicode (length 1)                                       |
| String                       | str/unicode                                                  |
| double                       | float                                                        |
| float                        | float                                                        |
| int                          | int                                                          |
| short                        | int                                                          |
| BigDecimal                   | decimal                                                      |
| BigInteger                   | long                                                         |
| any array                    | array if elements are primitive type (else tuple)            |
| Object[]                     | tuple (cannot contain self-references)                       |
| byte[]                       | bytearray                                                    |
| java.util.Date               | datetime.datetime                                            |
| java.util.Calendar           | datetime.datetime                                            |
| java.sql.Date                | datetime.date                                                |
| java.sql.Time                | datetime.time                                                |
| java.sql.Timestamp           | datetime.datetime                                            |
| Enum                         | the enum value as string                                     |
| java.util.Set                | set                                                          |
| Map, Hashtable               | dict                                                         |
| Vector, Collection           | list                                                         |
| Serializable                 | treated as a JavaBean, see below.                            |
| JavaBean                     | dict of the bean's public properties + `__class__` for the bean's type. |
| net.razorvine.pyro.PyroURI   | Pyro4.core.URI                                               |
| net.razorvine.pyro.PyroProxy | cannot be pickled.                                           |

