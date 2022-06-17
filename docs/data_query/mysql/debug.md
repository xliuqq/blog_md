# 使用



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
