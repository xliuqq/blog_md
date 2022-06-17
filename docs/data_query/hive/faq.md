# Hive FAQ

#### 问题：User root is not allowed to impersonate anonymous

**解决**：修改hadoop 配置文件 `etc/hadoop/core-site.xml`，加入如下配置项：

```xml
<property>
　　<name>hadoop.proxyuser.root.hosts</name>
　　<value>*</value>
</property>

<property>
　　<name>hadoop.proxyuser.root.groups</name>
　　<value>*</value>
</property>
```

然后，**动态刷新HDFS的用户权限配置**

```shell
bin/hdfs dfsadmin –refreshSuperUserGroupsConfiguration
bin/yarn rmadmin –refreshSuperUserGroupsConfiguration
```



#### 问题：Hive Schema version 1.2.0 does not match metastore's schema version 2.1.0

解决方案1：**修改mysql数据库中的Schame version**

```sql
# root身份访问mysql
mysql -u root -p

# 输入密码
Enter password:

# 使用hive数据库
# use hive

# 查看version表中Schema的版本
select * from version;

# 更新version表中schema版本
update VERSION set SCHEMA_VERSION='2.1.1' where VER_ID=1;
```

解决方案2：在`hvie-site.xml`中将`hive.metastore.schema.verification`参数设置为`false`

```xml
<property>
    <name>hive.metastore.schema.verification</name>
    <value>false</value>
</property>
```

