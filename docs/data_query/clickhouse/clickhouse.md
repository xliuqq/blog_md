# ClickHouse

**ClickHouse是一个用于联机分析(OLAP)的列式数据库管理系统(DBMS)，使用MPP架构。**

clickhouse不适合的场景

- Transactional workloads (OLTP)
- Key-value access with high request rate
- Blob or document storage
- Over-normalized data

**用户通过执行SQL语句，可以在ClickHouse中直接读取HDFS的文件，也可以将读取的数据导入到ClickHouse本地表。**