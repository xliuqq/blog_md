# FS

## [seaweedfs](https://github.com/seaweedfs/seaweedfs)

**compare to HDFS**

- ideal for serving **relatively smaller files quickly and concurrently**；
- "weed upload/download" tool to store extra large files by splitting them into manageable data chunks



**Compared to GlusterFS, Ceph**

| System          | File Metadata                    | File Content Read | POSIX     | REST API | Optimized for large number of small files |
| --------------- | -------------------------------- | ----------------- | --------- | -------- | ----------------------------------------- |
| SeaweedFS       | lookup volume id, cacheable      | O(1) disk seek    |           | Yes      | Yes                                       |
| SeaweedFS Filer | Linearly Scalable, Customizable  | O(1) disk seek    | FUSE      | Yes      | Yes                                       |
| GlusterFS       | hashing                          |                   | FUSE, NFS |          |                                           |
| Ceph            | hashing + rules                  |                   | FUSE      | Yes      |                                           |
| MooseFS         | in memory                        |                   | FUSE      |          | No                                        |
| MinIO           | separate meta file for each file |                   |           | Yes      | No                                        |



**compared to MoofseFS**

- designed for keeping large amounts (like several thousands) of very big files
- MooseFS Master Server keeps all meta data in memory. Same issue as HDFS namenode.



**Compared to MinIO**

- MinIO each file write will incur extra writes to corresponding meta file
- MinIO does not have optimization for lots of small files.
- MinIO has multiple disk IO to read one file. SeaweedFS has O(1) disk reads, even for erasure coded files.
- MinIO has full-time erasure coding. SeaweedFS uses replication on hot data for faster speed and optionally applies erasure coding on warm data.

