# Mybatis-plus





## 使用

### 返回自增主键

实体对象 主键`IdType`要设置为`AUTO` 表示数据库ID自增，然后使用Mapper进行插入时，返回的实体就会包含主键值。

```java
// employee 在插入后，就会自动插入主键到对象中
employeeService.saveOrUpdate(employee);
baseMapper.insert(employee);
```

