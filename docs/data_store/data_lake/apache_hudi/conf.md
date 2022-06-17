

### Clean

- hoodie.clean.automatic ：默认true
  - When enabled, the cleaner table service is invoked immediately after each commit, to delete older file slices. It's recommended to enable this, to ensure metadata and data storage growth is bounded.

- hoodie.clean.async ： 默认false





写入策略：

`write.insert.drop.duplicates`：默认false，INSERT操作是否丢弃重复记录，不丢弃会提升性能；

`write.insert.deduplicate`：默认true，INSERT操作是否去重，为false时直接写base file；