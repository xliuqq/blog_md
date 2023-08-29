# 配置

默认加载路径 PropertyKey.SITE_CONF_DIR，配置为

`${alluxio.conf.dir}/,${user.home}/alluxio,/etc/alluxio`





|                                          |      |                                                              |
| ---------------------------------------- | ---- | ------------------------------------------------------------ |
| alluxio.user.block.master.client.threads | 10   | The number of threads used by a block master client pool to talk to the block master. |
| alluxio.user.file.master.client.threads  | 10   | The number of threads used by a file master client to talk to the file master. |

## 网络

`alluxio.user.netty.data.transmission.enabled` 设置为 true 时，会直接使用 Netty 在 client 和 worker 中传输数据，避免了 grpc 的序列化和反序列化。



## 元数据

默认，Worker 会缓存元数据，设置 `alluxio.dora.client.metadata.cache.enabled`设置为 false 时，Client 会直接向底层的存储获取元数据。



## Security 配置

|                                      |        |                                                              |
| ------------------------------------ | ------ | ------------------------------------------------------------ |
| alluxio.security.authentication.type | SIMPLE | The authentication mode. Currently three modes are supported: **NOSASL, SIMPLE, CUSTOM**. The default value SIMPLE indicates that a simple authentication is enabled. Server trusts whoever the client claims to be. |
|                                      |        |                                                              |



## 调优

### 





