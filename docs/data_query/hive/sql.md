## SQL

# DDL

https://cwiki.apache.org/confluence/display/Hive/LanguageManual+DDL

- CREATE DATABASE/SCHEMA, TABLE, VIEW, FUNCTION, INDEX
- DROP DATABASE/SCHEMA, TABLE, VIEW, INDEX
- TRUNCATE TABLE
- ALTER DATABASE/SCHEMA, TABLE, VIEW
- MSCK REPAIR TABLE (or ALTER TABLE RECOVER PARTITIONS)
- SHOW DATABASES/SCHEMAS, TABLES, TBLPROPERTIES, VIEWS, PARTITIONS, FUNCTIONS, INDEX[ES], COLUMNS, CREATE TABLE
- DESCRIBE DATABASE/SCHEMA, table_name, view_name, materialized_view_name

### Create Table

### 

### Sort By & Order By & Cluster By & Distributed By

- Sort By 是单Reducer内有序的，Order By是全局有序；
- Distributed By保证相同列值的行会发送到同一个reducer中；
- *Cluster By* is a short-cut for both *Distribute By* and *Sort By*.