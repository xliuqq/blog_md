# FAQ

**ssh connection refused**

```shell
# 先查看ssh进程是否存在，不存在则启动
ps -ef | grep ssh
sudo /etc/init.d/ssh start
```

