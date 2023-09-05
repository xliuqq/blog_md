# 视频播放

> 示例代码：

## HLS

> *HLS* (HTTP Live Streaming）是Apple的动态码率自适应技术。主要用于PC和Apple终端的音视频服务。包括**一个m3u(8)的索引文件，TS媒体分片文件和key加密串文件**。

### m3u8格式

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
- **EXT-X-TARGETDURATION**：该标签指定了媒体**文件持续时间的最大值**，播放文件列表中的媒体文件在EXTINF标签中定义的持续时间必须小于或者等于该标签指定的持续时间。该标签在播放列表文件中必须出现一次。
- **EXT-X-MEDIA-SEQUENCE**：M3U8直播是的直播切换序列，当播放打开M3U8时，以这个标签的值作为参考，播放对应的序列号的切片。
- **EXT-X-KEY**：**视频的加密方式**，示例采用AES-128加密，**密钥在对应的URL路径中，无IV值**
  - METHOD 表示使用 AES-128 加解密，如果为 NONE 则表明不加密；
  - IV 没有，默认为 `00000000000000000000000000000000  `；
  - 其密钥地址为 `encryption.key`，通过该请求可以获得密钥`a041c4c59b89b6c37aa3703fbe3851cc`
  - Salt 没有；

- **EXTINF**：EXTINF为M3U8列表中每一个分片的duration，如上面例子输出信息中的第一片的duration为10秒。在EXTINF标签中，除了duration值，还可以包含可选的描述信息，主要为标注切片信息，使用逗号分隔开。

- **EXT-X-PLAYLIST-TYPE**：.
  - VOD，即为点播视频，服务器不能改变 Playlist ⽂件，换句话说就是该视频全部的 ts ⽂件已经被⽣成好；
  - EVENT，就是实时⽣成 m3u8 和 ts ⽂件。服务器不能改变或是删除 Playlist ⽂件中的任何部分，但是可以向该⽂件中增加新的⼀⾏内容。它的索引⽂件⼀直处于动态变化中，播放的时候需要不断下载⼆级 index ⽂件



### mp4 分片 m3u8

使用 ffmpeg 命令：

- `ffmpeg -y -i test-hoo.mp4 -hls_time 12 -hls_key_info_file enc.keyinfo -hls_playlist_type vod -hls_segment_filename "file%d.ts" playlist.m3u8`



### m3u8 下载

使用  ffmpeg 写在 m3u8 并合成 mp4；

- `ffmpeg -i http://a.com/test.m3u8 a.mp4`



*TODO（如何防止下载）*



### openssl 视频加解密

除了 m3u8 的加密方式之外，可以使用 openssl 对整个文件进行加解密。

- 服务端保存加密的文件，客户端获取到加密的文件，然后获取密钥，本地解析后再展示；
  - 效率比较慢？
  - 前端的视频组件支持使用 二进制流 作为输入；

#### 解密

`openssl aes-128-cbc -d -in seg-1-v1-a1.ts -out seg-1-v1-a1_decode.ts -nosalt -iv 00000000000000000000000000000000  -K a041c4c59b89b6c37aa3703fbe3851cc`

#### 加密

`openssl aes-128-cbc -e -in seg-1-v1-a1_decode.ts -out seg-1-v1-a1.ts -nosalt -iv 00000000000000000000000000000000  -K a041c4c59b89b6c37aa3703fbe3851cc`



### TS格式视频

*TS* 是高清格式，全称为 **MPEG2-TS**

- 从*视频*流的任一片段开始都可以独立解码。



## 前端

### [video.js](https://videojs.com/guides)

> Video.js - open source **HTML5** video player

- 原生支持 mp4, webm, ogg 格式；
- **flv** 格式需要引入 **videojs-flvjs** 和 **flv.js**；
- **rtmp**：video-js x6及以上版本不支持，因为[不支持`flash`](https://github.com/videojs/videojs-flash)；
- ... ...

插件：

- [`@videojs/http-streaming` ](https://github.com/videojs/http-streaming)：播放 hls 流插件，videojs 7版本以上安装时会自动安装；

### [Dash.js](https://github.com/Dash-Industry-Forum/dash.js)

> A reference client implementation for the playback of MPEG DASH via Javascript and compliant browsers.



## 后端

### Byte-Range

通过请求头的 [Range 等字段](../../cs/network/http.md#Range)，获取部分字节流；

- 适用场景：**前端可以识别相关的视频格式**；
- 局限：无法进行视频加密，可模拟请求获取数据并拼接形成视频；
- 使用：Spring 的 `ResourceHttpRequestHandler`的`handleRequest`用于处理：



### m3u8分片推送

> 通过 ffmpeg，用 m3u8 切片播放；

数据生成：使用 `org.bytedeco.javacv.FFmpegFrameRecorder`类进行生成

- 输入：`test.info` 文件

  - test.info内容如下:

    1）外部访问 key文件的地址

    2）执行时访问 key 的地址

    3）IV 值（16进制）

    ```txt
    http://127.0.0.1/m3u8/test.key
    ./video_play/resources/m3u8/test.key
    682f5033538cf71567e1bdb38f5f9a07
    ```

- 输入：`test.key` 文件
  - 一行表示密钥，如`n4DHLX7kMPeewvW3`（16 个 Byte，上述为 ASCII 文本形式的展示）

- 输出：`test.m3u8`

  ```ini
  #EXTM3U
  #EXT-X-VERSION:3
  #EXT-X-TARGETDURATION:60
  #EXT-X-MEDIA-SEQUENCE:0
  #EXT-X-KEY:METHOD=AES-128,URI="http://127.0.0.1/m3u8/test.key",IV=0x682f5033538cf71567e1bdb38f5f9a07
  #EXTINF:60.320000,
  test-0.ts
  #EXTINF:60.000000,
  test-1.ts
  ```

- 输出：`test-%n.ts` ，加密后的 ts 文件。



## 断点播放

> 记录之前观看的位置，并继续播放。

- 记录播放位置，写入 local storage；
- 刷新页面，获取播放位置；
- 从指定位置播放；

TODO：实现



## 直播协议

目前主流的几种直播协议

| 协议    | 传输方式 | 视频封装格式 | 延时 | 数据分段 | html播放                                  |
| ------- | -------- | ------------ | ---- | -------- | ----------------------------------------- |
| httpflv | http     | flv          | 低   | 连续     | 可通过html5解封包播放(flv.js)             |
| rtmp    | tcp      | flv tag      | 低   | 连续     | 不支持                                    |
| dash    | http     | ts文件       | 高   | 切片     | 可通过html5解封包播放(hls.js)             |
| hls     | $1       | mp4 3gp webm | 高   | 切片     | 如果dash文件列表是mp4webm文件，可直接播放 |

RTMP（Real Time Messaging Protocol）：**基于TCP**的，由Adobe公司为**Flash播放器**和服务器之间音频、视频传输开发的开放协议。

HLS（HTTP Live Streaming）：基于HTTP的，是Apple公司开放的音视频传输协议。

HTTP FLV：将**RTMP封装在HTTP**协议之上的，可以更好的穿透防火墙等。
