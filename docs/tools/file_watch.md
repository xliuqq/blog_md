# 文件监控

## Java

采用commons-io

```xml
<dependency>
  <groupId>commons-io</groupId>
  <artifactId>commons-io</artifactId>
  <version>2.6</version>
</dependency>
```

通过实现类 FileAlterationListener 或者继承 FileAlterationListenerAdaptor，通过观察者模式，实现对文件修改等事件的处理。

```java
// 创建过滤器
IOFileFilter directories = FileFilterUtils.and(FileFilterUtils.directoryFileFilter(),HiddenFileFilter.VISIBLE);
IOFileFilter files       = FileFilterUtils.and(FileFilterUtils.fileFileFilter(),FileFilterUtils.suffixFileFilter(".txt"));
IOFileFilter filter = FileFilterUtils.or(directories, files);

// 构造观察类主要提供要观察的文件或目录，当然还有详细信息的filter
FileAlterationObserver observer = new FileAlterationObserver("f:/1", filter);

FileListenerAdaptor listener = new FileListenerAdaptor();
// 为观察对象添加监听对象
observer.addListener(listener);
// 配置Monitor，第一个参数单位是毫秒，是监听的间隔；第二个参数就是绑定我们之前的观察对象（支持多个）。
FileAlterationMonitor fileMonitor = new FileAlterationMonitor(10000, observer);
// 启动开始监听
fileMonitor.start();
```

更加详细文件、目录的事件触发机制，TODO

## Python

采用跨平台的watchdog， pip install watchdog

```python
import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
 
class MyHandler(FileSystemEventHandler):
    def on_moved(self, event):
        super(MyHandler, self).on_moved(event)

    def on_deleted(self, event):
        super(MyHandler, self).on_deleted(event)

    def on_modified(self, event):
        super(MyHandler, self).on_modified(event)
        
    def on_created(self, event):
        super(MyHandler, self).on_moved(event)
 
if __name__ == "__main__":
    event_handler = MyHandler()
    observer = Observer()
    observer.schedule(event_handler, path='.', recursive=False)
    observer.start()
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()
```

