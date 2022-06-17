[toc]

# SQL

## 创建

### 库/表

> 处于两个不同库的表是不能进行JOIN操作的。

```sql
/* 创建一个名为power的库，这个库的数据将保留365天（超过365天将被自动删除），每10天一个数据文件，内存块数为4% */
CREATE DATABASE power KEEP 365 DAYS 10 BLOCKS 4;

USE power;  

/* 创建超级表 */
CREATE TABLE meters (ts timestamp, current float, voltage int, phase float) TAGS (location binary(64), groupdId int);
/* 创建表 */
CREATE TABLE d1001 USING meters TAGS ("Beijing.Chaoyang", 2);
/* 插入数据 */
INSERT INTO d1001 USING METERS TAGS ("Beijng.Chaoyang", 2) VALUES (now, 10.2, 219, 0.32);
```



## 连续查询（TODO）



## 数据订阅（TODO）



## 缓存（TODO）



## 报警监测（TODO）

https://github.com/taosdata/tdengine/blob/master/alert/README_cn.md