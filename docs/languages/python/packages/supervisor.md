# Supervisor: A Process Control System

> Supervisor is a client/server system that allows its users to control a number of processes on UNIX-like operating systems.

一个Python开发的通用的进程管理程序，可以管理和监控Linux上面的进程，能**将一个普通的命令行进程变为后台daemon，并监控进程状态，异常退出时能自动重启**，但它不能管理原本就是daemon的进程。

## 安装

```bash
pip  install supervisor
```

生成三个执行程序：

- *supervisortd* ：supervisor的守护进程服务（用于接收进程管理命令）
- *supervisorctl* ：客户端（用于和守护进程通信，发送管理进程的指令）
- *echo_supervisord_conf* ：生成初始配置文件程序

## 配置

### supervisor配置

创建目录，初始化配置文件，配置文件的搜索路径如下：

- `../etc/supervisord.conf` (Relative to the executable)
- `../supervisord.conf` (Relative to the executable)
- `CWD/supervisord.conf`(Current Work Directory)
- `CWD/etc/supervisord.conf`
- `/etc/supervisord.conf
- `/etc/supervisor/supervisord.conf (since Supervisor 3.3.0)`

```bash
mkdir /etc/supervisor 
echo_supervisord_conf >  /etc/supervisor/supervisord.conf
```

主配置文件参数

```ini
[unix_http_server]
file=/tmp/supervisor.sock   ; UNIX socket 文件，supervisorctl 会使用
;chmod=0700                 ; socket 文件的 mode，默认是 0700
;chown=nobody:nogroup       ; socket 文件的 owner，格式： uid:gid
  
;[inet_http_server]         ; HTTP 服务器，提供 web 管理界面
;port=127.0.0.1:9001        ; Web 管理后台运行的 IP 和端口，如果开放到公网，需要注意安全性
;username=user              ; 登录管理后台的用户名
;password=123               ; 登录管理后台的密码
  
[supervisord]
logfile=/tmp/supervisord.log ; 日志文件，默认是 $CWD/supervisord.log
logfile_maxbytes=50MB        ; 日志文件大小，超出会 rotate，默认 50MB
logfile_backups=10           ; 日志文件保留备份数量默认 10
loglevel=info                ; 日志级别，默认 info，其它: debug,warn,trace
pidfile=/tmp/supervisord.pid ; pid 文件
nodaemon=false               ; 是否在前台启动，默认是 false，即以 daemon 的方式启动
minfds=1024                  ; 可以打开的文件描述符的最小值，默认 1024
minprocs=200                 ; 可以打开的进程数的最小值，默认 200
  
; the below section must remain in the config file for RPC
; (supervisorctl/web interface) to work, additional interfaces may be
; added by defining them in separate rpcinterface: sections
[rpcinterface:supervisor]
supervisor.rpcinterface_factory = supervisor.rpcinterface:make_main_rpcinterface
  
[supervisorctl]
serverurl=unix:///tmp/supervisor.sock ; 通过 UNIX socket 连接 supervisord，路径与 unix_http_server 部分的 file 一致
;serverurl=http://127.0.0.1:9001 ; 通过 HTTP 的方式连接 supervisord
  
; 包含其他的配置文件
[include]
files = /etc/supervisor.d/*.conf    ; 可以是 *.conf 或 *.ini
```

#### rpcinterface

定制rpc的实现，成员函数可以通过`xmlrpc.client`进行RPC调用。

```ini
[rpcinterface:deployment]
data_dirs=/home/work/data
supervisor.rpcinterface_factory = deployment.rpcinterface:deployment_rpcinterface
```

client调用：

```python
self.proxy = xmlrpclib.ServerProxy('http://%s:%s@%s:%d' % (user, passwd, host, port))
message = self.proxy.deployment.stop(self.service, self.cluster, self.job, dict())
```



### 监控进程的配置

在`/etc/supervisord.d/`目录下新增一个conf文件

```ini
[program:project_name]
directory = 工作目录
command = 脚本的启动命令
autostart = true
autorestart = true
user = loanward
stdout_logfile = /data/log/pro_stdout.log
```

1. **project_name** 必须是唯一的，否则将会出现错乱
2. **directory** 填写你启动脚本时的工作目录，如果你使用脚本的绝对目录，那么directory可以不设置
3. **command**是执行脚本的命令，不需要使用nohup让其变为后代进程，supervisor会自动将其变为后台进程
4. **autostart** 配置为True， supervisor 启动时，任务跟随启动
5. **autorestart** 配置为True， 进程不存在时会自动重启， 默认重启3次，想修改重启次数，可以配置**startretries**， 设置最多重启的次数
6. **user** 设置启动任务时所用的用户，supervisor 有必要使用root账号启动，这样才有权限配置其他启动账号
7. **stdout_logfile** 配置输出日志

## 使用

### supervisorctl 

*supervisorctl* 是 *supervisord* 的一个命令行客户端工具，使用*supervisorctl*可以遍历的对进程进行管理，支持的命令如下

```text
supervisorctl status                         # 查看进程状态
supervisorctl stop project_name              # 终止某个进程
supervisorctl start project_name             # 启动某个进程
supervisorctl restart project_name           # 重启某个进程
supervisorctl reread                         # 更新配置，根据最新的配置启动程序，会启动已经运行的程序
supervisorctl update                         # 更新配置，重启配置有变化的进程
```





## 插件

### Supervisord支持的Event

- **PROCESS_STATE** 进程状态发生改变
- **PROCESS_STATE_STARTING** 进程状态从其他状态转换为正在启动(Supervisord的配置项中有startsecs配置项，是指程序启动时需要程序至少稳定运行x秒才认为程序运行正常，在这x秒中程序状态为正在启动)
- **PROCESS_STATE_RUNNING** 进程状态由正在启动转换为正在运行
- **PROCESS_STATE_BACKOFF** 进程状态由正在启动转换为失败
- **PROCESS_STATE_STOPPING** 进程状态由正在运行转换为正在停止
- **PROCESS_STATE_EXITED** 进程状态由正在运行转换为退出
- **PROCESS_STATE_STOPPED** 进程状态由正在停止转换为已经停止(exited和stopped的区别是exited是程序自行退出，而stopped为人为控制其退出)
- **PROCESS_STATE_FATAL** 进程状态由正在运行转换为失败
- **PROCESS_STATE_UNKNOWN** 未知的进程状态
- **REMOTE_COMMUNICATION** 使用Supervisord的RPC接口与Supervisord进行通信
- **PROCESS_LOG** 进程产生日志输出，包括标准输出和标准错误输出
- **PROCESS_LOG_STDOUT** 进程产生标准输出
- **PROCESS_LOG_STDERR** 进程产生标准错误输出
- **PROCESS_COMMUNICATION** 进程的日志输出包含 和
- **PROCESS_COMMUNICATION_STDOUT** 进程的标准输出包含 和
- **PROCESS_COMMUNICATION_STDERR**进程的标准错误输出包含 和
- **SUPERVISOR_STATE_CHANGE_RUNNING** Supervisord 启动
- **SUPERVISOR_STATE_CHANGE_STOPPING** Supervisord 停止
- **TICK_5** 每隔5秒触发
- **TICK_60** 每隔60秒触发
- **TICK_3600** 每隔3600触发
- **PROCESS_GROUP** Supervisord的进程组发生变化
- **PROCESS_GROUP_ADDED** 新增了Supervisord的进程组
- **PROCESS_GROUP_REMOVED** 删除了Supervisord的进程组

### [superlance](https://github.com/Supervisor/superlance)

> monitoring and controlling processes that run under [supervisor](http://supervisord.org/).

基于supervisor的事件机制实现的一系列命令行的工具集，监控并处理 supervisor 的事件，比如发邮件，特定状态下执行特定的脚本等。

