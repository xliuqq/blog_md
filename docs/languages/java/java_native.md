[toc]

# Java Native

## JNI

标准的Java native通信工具，但复杂度高，需要很多native code到java的回调。

## JNA

https://github.com/java-native-access/jna

- 基于libffi；
- 不需要JNI代码，只需要定义接口和函数（匹配
- 基于反射，数据转换从本地代码跳到Java代码非常昂贵；

## JNR

- 基于libffi，但使用**字节码生成**技术而不是反射；

<img src="https://serce.me/img/jnr-fuse/jnr.png" alt="jnr" style="zoom:33%;" />

### 类型转换

#### 基本类型

将 C 的类型和Java类型自动转换。

| C Type         | Java Type             | Size                                                         |
| -------------- | --------------------- | ------------------------------------------------------------ |
| `char`         | `byte`                | 8 bit integer                                                |
| `short`        | `short`               | 16 bit integer                                               |
| `int`          | `int`                 | 32 bit integer                                               |
| `long`         | `long`                | natural long, 32 bits on 32 bit systems, 64 bits on 64 bit systems |
| `float`        | `float`               | 32 bit floating point                                        |
| `double`       | `double`              | 64 bit floating point                                        |
| `const char *` | `String` or `Pointer` |                                                              |
| `void *`       | `Pointer` or `Buffer` |                                                              |
| int *          | IntByReference        | 其他float*有类似的类                                         |
| void **        | PointerByReference    |                                                              |
| C enum         | Mostly `int`          |                                                              |

#### 结构体/Union

c

```c
struct timespec {
    time_t tv_sec;
    long int tv_nsec;
};
```

Java

```java
// Union类似，集成Union类即可
public class Timespec extends Struct {
    // 结构体使用自己的基本类型
    // Struct types can be found as inner classes of the Struct class
    public Struct.SignedLong tv_sec = new Struct.SignedLong();
    public Struct.SignedLong tv_nsec = new Struct.SignedLong();

    // Necessary constructor that takes a Runtime
    public Timespec(jnr.ffi.Runtime runtime) {
        super(runtime);
    }

    // You can add your own methods of choice
    public void setTime(long sec, long nsec) {
        tv_sec.set(sec);
        tv_nsec.set(nsec);
    }
}
```



#### 枚举

c 枚举定义：

```c
enum fuse_buf_flags {
    FUSE_BUF_IS_FD    = (1 << 1),
    FUSE_BUF_FD_SEEK    = (1 << 2),
    FUSE_BUF_FD_RETRY    = (1 << 3),
};
```

Java对应定义（可以自动转换）

```java
public enum FuseBufFlags implements EnumMapper.IntegerEnum {
    FUSE_BUF_IS_FD(1 << 1),
    FUSE_BUF_FD_SEEK(1 << 2),
    FUSE_BUF_FD_RETRY(1 << 3);

    private final int value;

    FuseBufFlags(int value) {
        this.value = value;
    }

    @Override
    public int intValue() {
        return value;
    }
}
```

#### 函数类型

```c
typedef int MyCallback(void *data);

int setCallback(MyCallback *callback);
```

Java对应调用的写法：

```java
public interface ExampleLibrary {
    public interface MyCallback { // type representing callback
        @Delegate
        int invoke(Pointer data); // function name doesn't matter, it just needs to be the only function and have @Delegate
    }

    int setCallback(MyCallback callback);

}
```

### 示例

maven依赖

```xml
<dependency>
  <groupId>com.github.jnr</groupId>
  <artifactId>jnr-ffi</artifactId>
  <version>2.2.3</version>
</dependency>
```

示例代码：

```java
import jnr.ffi.*;
import jnr.ffi.types.pid_t;

/**
 * Gets the process ID of the current process, and that of its parent.
 */
public class Getpid {
    public interface LibC  {
        // 跟libc.so的getpid的本地方法匹配
        public @pid_t long getpid();
        public @pid_t long getppid();
    }

    public static void main(String[] args) {
        LibC libc = LibraryLoader.create(LibC.class).load("c");

        System.out.println("pid=" + libc.getpid() + " parent pid=" + libc.getppid());
    }
}
```

