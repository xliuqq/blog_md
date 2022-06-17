# 使用

## 安装

LDAP

K8s、Helm

MySQL

NFS

- `/cosdata/mlss-test`：ceph host path
  - `/cosdata/mlss-test/prophecis/etcd`
  - `/cosdata/mlss-test/prophecis/localstack`
  - `/cosdata/mlss-test/prophecis/mongo`
- `/cosdata/mlss-test/prophecis/minio`：Minio 挂载

- `/mlss/di/jobs/prophecis`：
  - 对应容器内的`/job`目录，用于？

- `/data/bdap-ss/mlss-data/tmp`：
  - 



Prophecis使用nfs来存储容器运行数据，需要挂载nfs

