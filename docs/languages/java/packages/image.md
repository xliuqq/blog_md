# Image

## 读写

BufferedImage ：图像类

ImageIO：图形读写工具类

```java
// 读取图片
File input = new File("ceshi.jpg");
BufferedImage image = ImageIO.read(input);

// 根据图片格式，创建ImageWrite类
Iterator<ImageWriter> writers =  ImageIO.getImageWritersByFormatName("jpg");
ImageWriter writer = (ImageWriter) writers.next();

// 创建对应的文件流
File compressedImageFile = new File("bbcompress.jpg");
OutputStream os =new FileOutputStream(compressedImageFile);
ImageOutputStream ios = ImageIO.createImageOutputStream(os);

// 设置输出流
writer.setOutput(ios);

// 设置参数（压缩模式和压缩质量）
ImageWriteParam param = writer.getDefaultWriteParam();
param.setCompressionMode(ImageWriteParam.MODE_EXPLICIT);
param.setCompressionQuality(0.01f);

// 将图像写出
writer.write(null, new IIOImage(image, null, null), param);

os.close();
ios.close();
writer.dispose();
```



## Compress

JDK

```java
// 设置参数（压缩模式和压缩质量）
ImageWriteParam param = writer.getDefaultWriteParam();
param.setCompressionMode(ImageWriteParam.MODE_EXPLICIT);
param.setCompressionQuality(0.01f);
```

压缩模式一共有四种：

- MODE_EXPLICIT 表示 ImageWriter 可以根据后续的 set 的附加信息进行平铺和压缩，比如说接下来的 `setCompressionQuality()` 方法。

`setCompressionQuality()` 方法的参数是一个 0-1 之间的数，0.0 表示尽最大程度压缩，1.0 表示保证图像质量很重要。

- 有损压缩：压缩质量应该控制文件大小和图像质量之间的权衡（例如，通过在写入 JPEG 图像时选择量化表）；
- 无损压缩：压缩质量可用于控制文件大小和执行压缩所需的时间之间的权衡（例如，通过优化行过滤器并在写入 PNG 图像时设置 ZLIB 压缩级别）。



OpenCV

```java
OpenCV.loadShared();

Mat src = Imgcodecs.imread(imagePath);

// 第一个参数 IMWRITE_JPEG_QUALITY 表示对图片的质量进行改变，第二个是质量因子，1-100，值越大表示质量越高。
MatOfInt dstImage = new MatOfInt(Imgcodecs.IMWRITE_JPEG_QUALITY, 10);

Imgcodecs.imwrite(imageOutPath, src, dstImage);
```



## 第三方

### Commons Imaging

Apache Commons Imaging，一个读取和写入各种图像格式的库，包括快速解析图像信息（如大小，颜色，空间，ICC配置文件等）和元数据。



### metadata-extractor

图像处理：Java用于图像读取和处理的包；

https://drewnoakes.com/code/exif/

https://github.com/drewnoakes/metadata-extractor

**Extracts Exif, IPTC, XMP, ICC and other metadata from image, video and audio files**

maven依赖

```xml
<dependency>
    <groupId>com.drewnoakes</groupId>
    <artifactId>metadata-extractor</artifactId>
    <version>2.13.0</version>
</dependency>
```



### [OpenCV](https://opencv.org/releases/)

由BSD许可证发布，可以免费学习和商业使用，提供了包括 C/C++、Python 和 Java 等主流编程语言在内的接口。OpenCV 专为计算效率而设计，强调实时应用，可以充分发挥多核处理器的优势。



openpnp 对 opencv 做简单的封装（将windows/linux等平台的opencv动态库放入jar包中，并且提供OpenCV.loadShared进行加载）。

```xml
<dependency>
 <groupId>org.openpnp</groupId>
 <artifactId>opencv</artifactId>
 <version>4.5.5</version>
</dependency>
```



