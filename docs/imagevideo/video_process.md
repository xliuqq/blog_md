[toc]

# 视频处理

## 抽取成图片

使用ffmpeg将图片抽取为多个图片

`ffmpeg -i /path/to/my/video.mp4 /path/to/output/video-frame-%d.png`

## 从图片合成

`ffmpeg -i video-frame-%0d.png -c:v libx264 -vf "fps=25,format=yuv420p" out.mp4`



## 播放器

[mpv](https://mpv.io/)

只播放音频部分：

`mpv --no-video {video_path} `