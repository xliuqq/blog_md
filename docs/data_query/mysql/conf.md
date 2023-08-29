# 配置



## 开启日志

查看是否是开启状态：

```
show global variables like '%general%';
```

查询日志开启

```sql
set global general_log_file='/tmp/general.lg';  #设置路径
set global general_log=on;  # 开启general log模式
set global general_log=off;  # 关闭general log模式
```

## 时区

```mysql
// 方法一：使用命令（优点：不需要重启MySQL服务，缺点：一旦MySQL服务被重启，设置就会消失）
mysql> set time_zone = '+8:00';
mysql> set global time_zone = '+8:00';
// 方法二：修改my.ini配置文件（优点：永久保存设置，缺点：需重启MySQL服务）
[mysqld]
// 设置默认时区
default-time_zone='+8:00'
```

