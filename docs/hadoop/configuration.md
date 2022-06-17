# Hadoop Configuration

new Configuration() 初始化过程中

- Configuration的类初始化会加载`core-default.xml`和`core-site.xml`作为defaultResource；
- ConfigUtil中会加载（这个不一定都会触发，目前所知FileSystem和UserGroupInformation会触发，由ReflectionUtils中的newInstance的setConf中会加载）
  - `mapred-default.xml`
  - `mapred-site.xml`
  - `yarn-default.xml`
  - `yarn-site.xml`



## 安全(Security)配置

| Parameter                        | Value      | Notes                                                        |
| :------------------------------- | :--------- | :----------------------------------------------------------- |
| `hadoop.security.authentication` | `kerberos` | `simple` : No authentication. (default)  `kerberos` : Enable authentication by Kerberos. |

## Delegation Token配置

**dfs.namenode.delegation.token.renew-interval**   

默认：86400000  (1 day)

**dfs.namenode.delegation.token.max-lifetime**  

默认：604800000  (7 days)

**dfs.namenode.delegation.key.update-interval**  

默认：86400000  (1 day)



**yarn.resourcemanager.delegation.key.update-interval**

默认：86400000  (1 day)

**yarn.resourcemanager.delegation.token.max-lifetime**

默认：604800000  (7 days)

**yarn.resourcemanager.delegation.token.renew-interval**

默认：86400000  (1 day)



## KMS

**hadoop.kms.authentication.type**

默认：simple，可选kerberos