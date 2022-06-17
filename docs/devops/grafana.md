# Grafana

默认的使用的是嵌入式SQLite，直接在本地存储，可以修改如下配置：

```ini
# Either "mysql", "postgres" or "sqlite3", it's your choice
type = sqlite3
host = 127.0.0.1:3306
name = grafana
user = root
# If the password contains # or ; you have to wrap it with triple quotes. Ex """#password;"""
password =
# Use either URL or the previous fields to configure the database
# Example: mysql://user:secret@host:port/database
url =

```



## 数据源

### Prometheus



### Graphite

对接 graphite-web



## OpenTSDB

对接 opentsdb http



## ElasticSearch

