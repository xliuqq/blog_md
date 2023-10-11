# A Distributed File System for Large Scale Container Platforms

## 摘要

CFS：大规模容器平台的分布式文件系统

- 支持顺序和随机文件访问，并优化大文件和小文件；



## 引言



关键特性：

- 通用的高性能存储（针对大/小文件）；
- 场景感知的复制因子；
- 无需数据迁移的存储扩容；
- Relaxed Posix 语义和元数据原子性；