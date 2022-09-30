# HDFS挂载本地

## NFS Gateway组件（官方）

> https://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-hdfs/HdfsNfsGateway.html

HDFS的NFS网关允许客户端挂载HDFS并通过NFS与其进行交互，就像它是本地文件系统的一部分一样。网关支持NFSv3。

安装HDFS后，用户可以：

- 在NFSv3客户端兼容的操作系统上通过其本地文件系统浏览HDFS文件系统。
- 在HDFS文件系统和本地文件系统之间上载和下载文件。
- 通过挂载点将数据直接传输到HDFS。 （支持文件追加，但不支持随机写入。）

### 0. 先决条件(版本: hadoop 3.2.2)

NFS网关机器需要具备运行HDFS client的所有配置，如Hadoop核心JAR文件和HADOOP_CONF目录。

### 1. 启动

启动NFS服务需要三个组件：rpcbind（或 portmap），mountd 和 nfsd。

- 停止机器自带的nfs， `service nfs stop`和`service rpcbind stop`；
- 启动hadoop portmap（需要linux root权限）：`$HADOOP_HOME/bin/hdfs --daemon start portmap`；
- 启动mountd 和 nfsd，` $HADOOP_HOME/bin/hdfs --daemon start nfs3`；

### 2. Hadoop配置

- 所有hadoop节点的`core-site.yaml`新增如下设置
  - 配置完以后在hadoop主节点依次执行:（hdfs 启动用户experiment）

- - `stop-dfs.sh`
  - `start-dfs.sh`

```yaml
<!-- 新增hdfs nfs gateway 配置  -->
<property>
  <name>hadoop.proxyuser.experiment.groups</name>
  <value>*</value>
  <description>
         The 'nfsserver' user is allowed to proxy all members of the 'users-group1' and
         'users-group2' groups. Note that in most cases you will need to include the
         group "root" because the user "root" (which usually belonges to "root" group) will
         generally be the user that initially executes the mount on the NFS client system.
         Set this to '*' to allow nfsserver user to proxy any group.
  </description>
</property>

<property>
  <name>hadoop.proxyuser.experiment.hosts</name>
  <value>*</value>
  <description>
         This is the host where the nfs gateway is running. Set this to '*' to allow
         requests from any hosts to be proxied.
  </description>
</property>

<property>
  <name>nfs.superuser</name>
  <value>experiment</value>
</property>
```

### 3. 启动NFS Gateway服务

#### 3.1 root用户启动portmap服务

- 所有hadoop节点

```bash
hdfs --daemon start portmap
```

#### 3.2 Hadoop启动用户启动nfs3服务

- 所有hadoop节点

```bash
hdfs --daemon start nfs3
```

#### 3.3 查看可挂载路径

```bash
showmount -e 172.16.2.135
```

#### 3.4 查看rbc端口是否启动正常

```bash
lsof -i:111
```

### 4. 挂载及取消挂载

#### 4.1 挂载

- 挂载目录需保证所有用户均有权限且为空目录

```bash
# ro 表明为只读
mount -t nfs -o vers=3,proto=tcp,nolock,noacl,sync,ro node135:/  /home/common/mnthdfs/
```

#### 4.2 取消挂载

```bash
umount -l /home/common/mnthdfs/
```

### 5. 停止NFS Gateway服务

#### 5.1 Hadoop启动用户停止nfs3服务

- 所有hadoop节点

```bash
hdfs --daemon stop nfs3
```

#### 5.2 root用户停止portmap服务

- 所有hadoop节点

```bash
hdfs --daemon stop portmap
```

## FUSE

