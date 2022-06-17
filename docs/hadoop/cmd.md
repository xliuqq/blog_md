

## Unable to load native-hadoop library for your platform

`hadoop checknative -a` 检查native library

**原因**：

Apache提供的hadoop本地库是32位的，而在64位的服务器上就会有问题，因此需要自己编译64位的版本。

**解决**：

- http://dl.bintray.com/sequenceiq/sequenceiq-bin/  下载对应的编译版本；

- 将准备好的64位的lib包解压到已经安装好的hadoop安装目录的lib/native 和 lib目录下；

