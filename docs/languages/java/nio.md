[toc]

# Java NIO

Java中跟zero copy相关的主要集中在FileChannel和MappedByteBuffer中。

网络通讯框架Netty4中跟zero copy相关的则主要集中在FileRegion和CompositeByteBuf中。



## 零拷贝

Zero拷贝的细节，见[该文档](../../os/IO.md#Linux OS的零拷贝（发送文件为例）)。

### MappedByteBuffer

java nio提供的FileChannel提供了map()方法，该方法可以在一个打开的文件和MappedByteBuffer之间建立一个虚拟内存映射，MappedByteBuffer继承于ByteBuffer，类似于一个基于内存的缓冲区，只不过该对象的数据元素存储在磁盘的一个文件中；

示例代码

```java
public class MappedByteBufferTest {
    public static void main(String[] args) throws Exception {
        File file = new File("D://db.txt");
        long len = file.length();
        byte[] ds = newbyte[(int) len];
        MappedByteBuffer mappedByteBuffer = new FileInputStream(file).getChannel().map(FileChannel.MapMode.READ_ONLY, 0, len);
        for (int offset = 0; offset < len; offset++) {
            byte b = mappedByteBuffer.get();
            ds[offset] = b;
        }
        Scanner scan = new Scanner(new ByteArrayInputStream(ds)).useDelimiter(" ");
        while (scan.hasNext()) {
            System.out.print(scan.next() + " ");
        }
    }
}
```

MappedByteBuffer本身是一个抽象类，其实这里真正实例话出来的是DirectByteBuffer。

### DirectByteBuffer

DirectByteBuffer 继承于MappedByteBuffer，不是JVM堆内存。

```java
ByteBuffer directByteBuffer = ByteBuffer.allocateDirect(100);
```

### Channel-to-Channel传输

经常需要从一个位置将文件传输到另外一个位置，FileChannel提供了transferTo()方法用来提高传输的效率：

```java
作者：Java编程宇宙
链接：https://www.zhihu.com/question/498972987/answer/2246203635
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

public class ChannelTransfer {
    public static void main(String[] argv) throws Exception {
        String files[]=new String[1];
        files[0]="D://db.txt";
        catFiles(Channels.newChannel(System.out), files);
    }

    private static void catFiles(WritableByteChannel target, String[] files)
            throws Exception {
        for (int i = 0; i < files.length; i++) {
            FileInputStream fis = new FileInputStream(files[i]);
            FileChannel channel = fis.getChannel();
            // 重点
            channel.transferTo(0, channel.size(), target);
            channel.close();
            fis.close();
        }
    }
}
```

### netty中零拷贝

#### 避免数据流经用户空间

即[OS中的零拷贝]([该文档](../../os/IO.md#Linux OS的零拷贝（发送文件为例）))：

- Netty的文件传输调用**FileRegion包装的transferTo**方法，可以直接将[文件缓冲区](https://www.zhihu.com/search?q=文件缓冲区&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"article"%2C"sourceId"%3A76059333})的数据发送到目标Channel，避免通过循环write方式导致的内存拷贝问题，FileRegion底层调用**NIO FileChannel**的transferTo函数；

#### 避免数据从JVM Heap到C Heap的拷贝

在JVM层面，每当程序需要执行一个I/O操作时，都需要将数据先从**JVM管理的堆内存**复制到**使用C malloc()或类似函数分配的Heap内存**中才能够触发系统调用完成操作，这部分内存站在Java程序的视角来看就是堆外内存，但是**以操作系统的视角来看其实都属于进程的堆区**

- Netty的接收和发送ByteBuffer使用**直接内存进行Socket读写**，不需要进行**字节缓冲区**的二次拷贝。
  - 如果使用JVM的堆内存进行Socket读写，JVM会将堆内存Buffer拷贝一份到直接内存中，然后才写入Socket中。相比于使用直接内存，消息在发送过程中多了一次缓冲区的内存拷贝；

#### 减少数据在用户空间的多次拷贝

- Netty提供CompositeByteBuf类，可以将多个ByteBuf合并为一个**逻辑上的ByteBuf**, 避免了各个ByteBuf之间的拷贝；
- 通过**wrap操作**, 我们可以将byte[]数组、ByteBuf、ByteBuffer等包装成一个Netty ByteBuf对象, 进而避免拷贝操作；
- ByteBuf支持slice操作，可以将ByteBuf分解为**多个共享同一个存储区域的ByteBuf**，避免内存的拷贝。