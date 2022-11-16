# 序列化

## JDK

序列版本UID（serial version UID），流的唯一标识符：

- 默认根据类名称、实现接口、成员等信息自动生成，因此很容易出现兼容性问题；



### Serializable

#### readObject & writeObject

Java实现序列化，实现`Serializable`接口，该接口中没有任何的方法，但是要实现自定义的序列化方法时，需要重载`readObject`, `writeObject`和`readObjectNoData`且都是private

- 当`ObjectOutputStream`调用`writeObject`时，会根据`instanceof Serializable`，以及类的`ObjectStreamClass`（硬编码读取该三个方法），判断是否调用用户实现的方法还是默认的序列化方法；

  

除了基本的`writeObject`和`readObject`之外，以下三个函数也可以控制序列化和反序列化：

- **readObjectNoData()** :用于初始化反序列化对象，当发生一些情况导致反序列化对象不能获得数据时调用；

- **writeReplace()** ：指派其他对象写入序列化的流中；

- **readResolve()**：返回的对象替换反序列化创建的实例；



#### writeReplace & readResolve

Serializable还有两个标记接口方法可以实现序列化对象的替换，即`writeReplace`和`readResolve`:

- `Object writeReplace() throws ObjectStreamException`
  - 序列化时会**先调用`writeReplace`方法将当前对象替换成另一个对象**（该方法会返回替换后的对象，替换后的对象必须可序列化）并将其写入流中，调用过程为 `writeObject(writeReplace())`
- `Object readResolve() throws ObjectStreamException`
  - **`readResolve`会在`readObject`调用之后自动调用**，`readResolve`里再对该对象进行一定的修改，而最终修改后的结果将作为`readObject`的返回结果；
- 应用就是**保护性恢复单例**的对象
  - 对单例对象，重写`readResolve`方法，保证返回的是系统中唯一的单例。

### 枚举类型的序列化

在枚举类型的序列化和反序列化上，Java做了特殊的规定，即**在序列化的时候Java仅仅是将枚举对象的name属性输出到结果中，反序列化的时候则是通过java.lang.Enum的valueOf方法来根据名字查找枚举对象**。

同时，**编译器是不允许任何对这种序列化机制的定制**，因此禁用了writeObject、readObject、readObjectNoData、writeReplace和readResolve等方法。

### Lambda 表达式的序列化

```scala
val writeReplace = closure.getClass.getDeclaredMethod("writeReplace")
writeReplace.setAccessible(true)
writeReplace.invoke(closure).asInstanceOf[java.lang.invoke.SerializedLambda]

// lambda生成的字节码
@FunctionalInterface
 interface Print<T> {
   public void print(T x);
 }
 public class Lambda {  
   public static void PrintString(String s, Print<String> print) {
     print.print(s);
   }
   private static void lambda$0(String x) {
     System.out.println(x);
   }
   final class $Lambda$1 implements Print{
     @Override
     public void print(Object x) {
       lambda$0((String)x);
     }
   }
   public static void main(String[] args) {
     PrintString("test", new Lambda().new $Lambda$1());
   }
 }
```

## Kryo

> [Java 序列化框架](https://github.com/EsotericSoftware/kryo)

###  

### 线程

非线程安全，Each thread should have its own Kryo, Input, and Output instances.



### Demo



 

