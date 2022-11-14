# Linux系统中的日志管理

/run：是一个**临时文件系统**（[tmpfs](./ramdisk.md)），存储系统启动以来的信息。当系统重启时，这个目录下的文件应该被删掉或清除。



## Journald

收集来自内核、系统早期启动阶段的日志、**系统守护进程**在启动和运行中的标准输出和错误信息，还有 **syslog** 的日志。

journald日志写入**二进制文件**。

存储位置：

- `volatile`表示仅保存在内存中， 也就是仅保存在 `/run/log/journal` 目录中(将会被自动按需创建)；
-  `persistent`表示优先保存在磁盘上， 也就优先保存在 `/var/log/journal` 目录中(将会被自动按需创建)；
- "`auto`"(默认值) 与 "`persistent`" 类似， 但不自动创建 `/var/log/journal` 目录， 因此可以根据该目录的存在与否决定日志的保存位置。



**会根据文件大小，设置滚动策略，以及总的文件大小，进行清除策略**。



## rsyslog 

rsyslog作为传统的系统日志服务，把所有收集到的日志都记录到`/var/log/`目录下的各个日志文件中。



## logrorate

日志文件管理工具，**删除旧日志**并**创建新的日志**，以及**压缩日志**和发送日志到email；

```shell
$ logrorate -s <statefile> <conffile>
```

其配置文件为`/etc/logrotate.conf`，主要参数：/var/log/*.log     { args }

示例

```conf

```



- compress | nocompress：gzip压缩转储后的日志；

- copytruncate      | nocopytruncate ： 打开中的日志文件，当前日志备份并截断（丢失日志的风险）；

- ifempty | notifemply：空文件也转储（默认）；

- rotate N ：日志文件删除前的转储次数，0指没有备份，5指保留5个备份；

- create mode owner group：转储文件，使用指定的文件模式创建新的日志文件；

- prerotate/endscript：转储前需要执行的命令，关键字单独成行；

- postrotate/endscript：转储后需要执行的命令，关键字单独成行；

- missingok |nomissingok：日志文件不存在，继续下一个而不报错；

- daily | weekly |monthly：转储周期；

- size=19：日志文件到指定大小再转储，默认bytes，可选K和M；

  

Linux写文件是根据inode编号，**重命名文件不影响文件的读写**；

默认，创建一个新的日志文件给程序输出日志，通过某种机制通知程序，如 nginx 支持 kill -USR1，需要程序支持；

配合cron使用，如果 logrotate 配置了daily，则一天内即使cron重复使用，也不会输出两份备份；
