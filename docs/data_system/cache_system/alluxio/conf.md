# 配置

默认加载路径 PropertyKey.SITE_CONF_DIR，配置为

`${alluxio.conf.dir}/,${user.home}/alluxio,/etc/alluxio`



|                                          |      |                                                              |
| ---------------------------------------- | ---- | ------------------------------------------------------------ |
| alluxio.user.block.master.client.threads | 10   | The number of threads used by a block master client pool to talk to the block master. |
| alluxio.user.file.master.client.threads  | 10   | The number of threads used by a file master client to talk to the file master. |





## Security 配置

|                                      |        |                                                              |
| ------------------------------------ | ------ | ------------------------------------------------------------ |
| alluxio.security.authentication.type | SIMPLE | The authentication mode. Currently three modes are supported: **NOSASL, SIMPLE, CUSTOM**. The default value SIMPLE indicates that a simple authentication is enabled. Server trusts whoever the client claims to be. |
|                                      |        |                                                              |