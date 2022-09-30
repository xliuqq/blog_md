[toc]

# HDFS 权限

HDFS的权限模型类似POSIX模型，文件/目录和用户及用户组关联，对所属用户，用户组的不同用户，其它用户是分开的权限。

- 文件，`r`，`w`
- 目录，`r`（列出目录下的内容）， `w`（创建和删除文件/目录），`x`（访问目录的子级）

没有setuid和setgid，因为没有可执行文件的概念。对于目录，没有setuid或setgid bits目录作为简化

可以在目录上**设置粘滞位**，防止除超级用户、目录所有者或文件所有者之外的任何人删除或移动目录中的文件。为文件设置粘滞位无效。

**创建文件或目录时，其所有者是客户端进程的用户标识，其组是父目录的组**（BSD规则）

HDFS还提供了对POSIX **acl（访问控制列表）的可选支持**，以使用针对特定命名用户或命名组的更细粒度规则扩展文件权限。

访问HDFS的每个**客户端进程都有一个由用户名和组列表组成的两部分标识**，访问目录/文件时会进行权限检查。

## 用户标识

`hadoop.security.authentication`

- simple：默认，客户端进程的标识由主机操作系统确定。在类Unix系统中，用户名相当于“whoami”。
- kerboers：客户端进程的标识由其Kerberos凭据确定，用户可以使用kinit实用程序获取Kerberos票证授予票证（TGT），并使用klist确定其当前主体。

## 用户组映射

一旦按上述方式确定用户名，组列表将由组映射服务（`hadoop.security.group.mapping`）确定

默认：`org.apache.hadoop.security.JniBasedUnixGroupsMappingWithFallback`

- 用于ACL的用户到组映射（获取给定用户的组）的类；
- JNI可用，则实现将使用hadoop中的API来解析用户的组列表。如果JNI不可用，则使用shell实现（`bash -c groups $user`；

## 权限检查

Path组成

- **Ownership**：是否检查调用者是路径的所有者，当改变ownership和permission元数据需要；
- **Parent**：请求路径的父级目录，路径`/foo/bar/baz`的父路径是`/foo/bar`
- **Ancestor**：请求路径最后存在的部分，对`/foo/bar/baz`，ancestor是`/foo/bar`如果存在，是`/foo`如果`/foo/bar`不存在但`/foo`存在；
- **Final**：请求路径的最后部分，路径`/foo/bar/baz`的final路径部分是`/foo/bar/baz`；
- **Sub-tree**：如果Path是目录，则sub-tree部分是该目录本身和它递归的所有子目录；

权限检查表如下：

| Operation             | Ownership | Parent          | Ancestor            | Final                               | Sub-tree             |
| :-------------------- | :-------- | :-------------- | :------------------ | :---------------------------------- | :------------------- |
| append                | NO        | N/A             | N/A                 | WRITE                               | N/A                  |
| concat                | NO [2]    | WRITE (sources) | N/A                 | READ (sources), WRITE (destination) | N/A                  |
| create                | NO        | N/A             | WRITE               | WRITE [1]                           | N/A                  |
| createSnapshot        | YES       | N/A             | N/A                 | N/A                                 | N/A                  |
| delete                | NO [2]    | WRITE           | N/A                 | N/A                                 | READ, WRITE, EXECUTE |
| deleteSnapshot        | YES       | N/A             | N/A                 | N/A                                 | N/A                  |
| getAclStatus          | NO        | N/A             | N/A                 | N/A                                 | N/A                  |
| getBlockLocations     | NO        | N/A             | N/A                 | READ                                | N/A                  |
| getContentSummary     | NO        | N/A             | N/A                 | N/A                                 | READ, EXECUTE        |
| getFileInfo           | NO        | N/A             | N/A                 | N/A                                 | N/A                  |
| getFileLinkInfo       | NO        | N/A             | N/A                 | N/A                                 | N/A                  |
| getLinkTarget         | NO        | N/A             | N/A                 | N/A                                 | N/A                  |
| getListing            | NO        | N/A             | N/A                 | READ, EXECUTE                       | N/A                  |
| getSnapshotDiffReport | NO        | N/A             | N/A                 | READ                                | READ                 |
| getStoragePolicy      | NO        | N/A             | N/A                 | READ                                | N/A                  |
| getXAttrs             | NO        | N/A             | N/A                 | READ                                | N/A                  |
| listXAttrs            | NO        | EXECUTE         | N/A                 | N/A                                 | N/A                  |
| mkdirs                | NO        | N/A             | WRITE               | N/A                                 | N/A                  |
| modifyAclEntries      | YES       | N/A             | N/A                 | N/A                                 | N/A                  |
| removeAcl             | YES       | N/A             | N/A                 | N/A                                 | N/A                  |
| removeAclEntries      | YES       | N/A             | N/A                 | N/A                                 | N/A                  |
| removeDefaultAcl      | YES       | N/A             | N/A                 | N/A                                 | N/A                  |
| removeXAttr           | NO [2]    | N/A             | N/A                 | WRITE                               | N/A                  |
| rename                | NO [2]    | WRITE (source)  | WRITE (destination) | N/A                                 | N/A                  |
| renameSnapshot        | YES       | N/A             | N/A                 | N/A                                 | N/A                  |
| setAcl                | YES       | N/A             | N/A                 | N/A                                 | N/A                  |
| setOwner              | YES [3]   | N/A             | N/A                 | N/A                                 | N/A                  |
| setPermission         | YES       | N/A             | N/A                 | N/A                                 | N/A                  |
| setReplication        | NO        | N/A             | N/A                 | WRITE                               | N/A                  |
| setStoragePolicy      | NO        | N/A             | N/A                 | WRITE                               | N/A                  |
| setTimes              | NO        | N/A             | N/A                 | WRITE                               | N/A                  |
| setXAttr              | NO [2]    | N/A             | N/A                 | WRITE                               | N/A                  |
| truncate              | NO        | N/A             | N/A                 | WRITE                               | N/A                  |

[1] 仅当调用使用overwrite选项且路径处存在现有文件时，才需要在创建期间对final path组件进行写访问；

[2] 如果设置了粘滞位，则检查父目录上写权限的任何操作也会检查所有权。

[3] 调用setOwner更改拥有文件的用户需要HDFS超级用户访问权限。更改组不需要HDFS超级用户访问权限，但调用方必须是文件的所有者和指定组的成员。

**实现注意事项**

- 每个文件或目录操作都将完整路径名传递给NameNode，并且权限检查将沿着每个操作的路径应用；

- 客户端框架将隐式地将用户标识与到NameNode的连接关联起来，从而减少对现有客户端API的更改；
- 删除文件不会撤消已经知道文件块的客户端的访问；
- 随着权限的增加，客户端对文件的访问可能在请求之间被撤消；
- 更改权限不会撤消已经知道文件块的客户端的访问。

## 超级用户

超级用户是与**NameNode进程本身具有相同标识**的用户。

管理员可以使用配置参数标识可分辨组。如果已设置，则此组的成员也是超级用户；

## ACLs (Access Control Lists)

ACL的解释说明见 linux/acls.md  

最佳实践是依赖传统的权限位来实现大多数权限需求，并定义少量的acl以使用一些特殊规则来扩充权限位，减少NameNode增加的内存开销。

## 配置参数

- `dfs.permissions.enabled`：默认 true

  false则关闭权限检查，但所有其他行为都保持不变，不会更改文件或目录的模式、所有者或组。

  无论权限是打开还是关闭，chmod、chgrp、chown和setfacl总是检查权限。

- `hadoop.http.staticuser.user(dfs.web.ugi)`：默认 None，可设置如 webuser,webgroup

  web服务器要使用的用户名。将此设置为超级用户的名称将允许任何web客户端查看所有内容。

  将此更改为其他未使用的用户标识允许web客户端仅使用“other”权限查看可见的内容。可以将其他组添加到逗号分隔的列表中。

- `dfs.permissions.superusergroup`：默认 supergroup

  super-users的组名。

- `fs.permissions.umask-mode`：默认 022

  创建文件和目录时使用的umask，对于目录是755，对于文件是644。

- `dfs.cluster.administrators`：默认None，设置用户或组，如 user1,user2 group1,group2

  指定为ACL的群集的管理员，空格用来分割用户列表和组列表，支持通配符*

- `dfs.namenode.acls.enabled`：默认false

  当ACLs禁用时，NameNode会拒绝设置ACL。