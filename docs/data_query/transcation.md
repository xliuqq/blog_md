# 事务



## 锁机制

在写入数据库的时候需要有锁，比如同时写入数据库的时候会出现丢数据，那么就需要锁机制。

### 乐观锁

> CAS + 版本号
>
> - CAS 保证原子性，同一时间的更改只有一个生效

适用于**写少读多**，采用**版本号**的方式，即当前版本号如果对应上了就可以写入数据，如果判断当前版本号不一致，那么就不会更新成功

```sql
# 同样的语句并发执行不会执行
update goods set num = num - 1, version = version - 1 where id = 1 and version = 0;
```



### 悲观锁

适用于**写多读少**。

```sql
# 排它锁，其它事务不可读写
select num from goods where id = 1 for update;
update goods set num=num-1 where id = 1;
```
