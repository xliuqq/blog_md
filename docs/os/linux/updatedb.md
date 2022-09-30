# Updatedb/mlocate

updatedb 命令用于更新 locate 命令使用的数据库。

updatedb 作用的数据库名称由配置文件（一般为 /etc/updatedb.conf）或命令行参数确定（一般为 /var/lib/mlocate/mlocate.db），由系统每日执行 updatedb 更新一次，更新脚本一般为 /etc/cron.daily/mlocate，在配置文件 /etc/anacrontab 中被调用，由 anacron 每天运行一次。

