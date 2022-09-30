[toc]

# 配置



Bookie的内存分配建议：

- 操作系统保留：2GB；
- JVM：占 1 / 2；
  - Heap Memory ：1 / 3
  - Direct Memory ：2 / 3
- FileSystem Page Cache ：占 1 / 2