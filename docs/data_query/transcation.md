# 事务



## 锁机制

在写入数据库的时候需要有锁，比如同时写入数据库的时候会出现丢数据，那么就需要锁机制。

### 乐观锁

适用于**写少读多**，采用**版本号**的方式，即当前版本号如果对应上了就可以写入数据，如果判断当前版本号不一致，那么就不会更新成功

### 悲观锁

适用于**写多读少**。

