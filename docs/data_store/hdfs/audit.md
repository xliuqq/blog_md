# Audit

HDFS 审计日志，在`DefaultAuditLogger`中的`logAuditEvent`方法中：

```
2015-09-09 05:29:54,727 INFO FSNamesystem.audit: allowed=true	ugi=data (auth:SIMPLE)	ip=/192.128.10.15	cmd=open	
src=/user/data/.staging/job_1441718170682_2949/job.jar	dst=null	perm=null	proto=rpc
```

上面审计日志的格式重点关注（时间, ugi用户信息, ip地址信息, cmd操作）

```properties
#
# hdfs audit logging
#
hdfs.audit.logger=INFO,RFAAUDIT
hdfs.audit.log.maxfilesize=256MB
hdfs.audit.log.maxbackupindex=100
log4j.logger.org.apache.hadoop.hdfs.server.namenode.FSNamesystem.audit=${hdfs.audit.logger}
log4j.additivity.org.apache.hadoop.hdfs.server.namenode.FSNamesystem.audit=false
log4j.appender.RFAAUDIT=org.apache.log4j.RollingFileAppender
log4j.appender.RFAAUDIT.File=${hadoop.log.dir}/hdfs-audit.log
log4j.appender.RFAAUDIT.layout=org.apache.log4j.PatternLayout
log4j.appender.RFAAUDIT.layout.ConversionPattern=%d{ISO8601} %p %c{2}: %m%n
log4j.appender.RFAAUDIT.MaxFileSize=${hdfs.audit.log.maxfilesize}
log4j.appender.RFAAUDIT.MaxBackupIndex=${hdfs.audit.log.maxbackupindex}
```

写个程序做文本解析，进行count计数，以及分钟级别qps请求量的趋势统计，分用户级别的请求量分析。