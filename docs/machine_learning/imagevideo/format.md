[toc]

# 格式

## 图像

Ogg全称是OGGVobis(oggVorbis)是一种音频压缩格式，类似于MP3等的音乐格式。Ogg是完全免费、开放和没有专利限制的。



## M3U8格式



格式:

- EXT-X-VERSION：协议版本号，该标签与流媒体的兼容性相关；

- **[EXT-X-TARGETDURATION](https://datatracker.ietf.org/doc/html/rfc8216#page-22)**：表示每个视频分段最大的时长（单位秒）；
- **[EXT-X-KEY](https://datatracker.ietf.org/doc/html/rfc8216#page-15)**：媒体片段可以进行加密，而该标签可以指定解密方法。
  - **METHOD**：值可为`NONE`，`AES-128`，`SAMPLE-AES`当中；
  - **URI**：指定密钥路径（16 字节的数据）；该键是必须参数，除非 **METHOD** 为`NONE`。
  - **IV**：该值是一个 128 位的十六进制数值
    - AES-128 要求使用相同的 16字节 IV 值进行加密和解密；
    - 属性列表出现 **IV**，则使用该值；如果未出现，则默认使用媒体片段序列号作为其 **IV** 值，使用大端字节序，往左填充 0 直到序列号满足 16 字节（128 位）。
- **EXTINF**：表示其后 URL 指定的媒体片段时长（单位为秒）。每个 URL 媒体片段之前必须指定该标签

```tx
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-ALLOW-CACHE:YES
#EXT-X-TARGETDURATION:6
#EXT-X-KEY:MEATHOD=AES-128,URI="Njk2NTAzZGEtOGMxZS00NjJkLWI5NTItMGQ3M2M4MDQzMjU5czBvVTJYUUNkRFRNa1ZZcWd0RGQrYnpTOWpPdGVRNEpBQUFBQUFBQUFBQTl4TDFFOEgreGV5OWFKdjAwaWhoeHJTVllTcWV0VWlYT093UWNUVjJwbExTUWx5OW9SSWQz"
#EXT-X-MEDIA-SEQUENCE:0
#EXTINF:6.000000,e88f7f24c8e5fb8ec1720bbac2b176fd-ld-encrypt-stream-00004.ts
```

