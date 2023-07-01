# 视频播放

## m3u8格式

M3U8文件是指UTF-8编码格式的M3U文件。M3U文件是记录了一个**索引纯文本**文件，打开它时播放软件并不是播放它，而是根据它的索引找到对应的音视频文件的网络地址进行在线播放。

```
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-MEDIA-SEQUENCE:35232
#EXT-X-TARGETDURATION:10
#EXT-X-KEY:METHOD=AES-128,URI="encryption.key"
#EXTINF:10.000,
```

- **EXTM3U**：这个是M3U8文件必须包含的标签，并且必须在文件的第一行，所有的M3U8文件中必须包含这个标签。
- **EXT-X-VERSION**：M3U8文件的版本，常见的是3（目前最高版本应该是7）。
- **EXT-X-TARGETDURATION**：该标签指定了媒体文件持续时间的最大值，播放文件列表中的媒体文件在EXTINF标签中定义的持续时间必须小于或者等于该标签指定的持续时间。该标签在播放列表文件中必须出现一次。
- **EXT-X-MEDIA-SEQUENCE**：M3U8直播是的直播切换序列，当播放打开M3U8时，以这个标签的值作为参考，播放对应的序列号的切片。
- **EXT-X-KEY**：视频的加密方式，示例采用AES-128加密，密钥在对应的URL路径中，无IV值
- **EXTINF**：EXTINF为M3U8列表中每一个分片的duration，如上面例子输出信息中的第一片的duration为10秒。在EXTINF标签中，除了duration值，还可以包含可选的描述信息，主要为标注切片信息，使用逗号分隔开。

### 视频加密的解密

`openssl aes-128-cbc -d -in seg-1-v1-a1.ts -out seg-1-v1-a1_decode.ts -nosalt -iv 00000000000000000000000000000000  -K a041c4c59b89b6c37aa3703fbe3851cc`
