# Mybatis-plus



## Mysql

```sql
# 会过滤掉 NULL 值
id != ''
```



## 使用

### 返回自增主键

实体对象 主键`IdType`要设置为`AUTO` 表示数据库ID自增，然后使用Mapper进行插入时，返回的实体就会包含主键值。

```java
// employee 在插入后，就会自动插入主键到对象中
employeeService.saveOrUpdate(employee);
baseMapper.insert(employee);
```



### 字段为null时不更新

这两个方法，不会对实体中值为Null的属性（字段）进行更新。

```java
mapper.updateById(entity); 
```

Mybatis-plus默认配置中，对于参数字段为null时，会自动忽略。导致进行update操作时，无法将字段更新为null值。

- 单个配置，每个参数上加注解。（但是如果其它场景不传入该值时，会导致会被更新，可能会出问题）

  ```java
  @TableField(updateStrategy = FieldStrategy.IGNORED)
  ```

- 使用**LambdaUpdateWrapper**（推荐）

  ```java
  LambdaUpdateWrapper<BizFile> lambdaUpdateWrapper = new LambdaUpdateWrapper<>();
  //过滤条件
  lambdaUpdateWrapper.eq(BizFile::getId, bizFile.getId());
  
  //下面为设置值  		
  //由于parentId会为空，所以要使用LambdaUpdateWrapper
  lambdaUpdateWrapper.set(BizFile::getParentId, parentId);
  lambdaUpdateWrapper.set(BizFile::getPath, newDirPath);
  
  //更新
  this.update(null, lambdaUpdateWrapper);
  ```

  
