# 配置

Hive全量的配置信息：

https://cwiki.apache.org/confluence/display/Hive/Configuration+Properties#ConfigurationProperties-HiveServer2

## HiveServer2

### 性能

- **hive.server2.thrift.min.worker.threads** – Minimum number of worker threads, default 5.
- **hive.server2.thrift.max.worker.threads** – Maximum number of worker threads, default 500.

### 安全

- **hive.server2.authentication** : default NONE. Options are NONE (uses plain SASL), NOSASL, KERBEROS, LDAP, PAM and CUSTOM.



## Log

### Perf Log

- **log4j.logger.org.apache.hadoop.hive.ql.log.PerfLogger=DEBUG**