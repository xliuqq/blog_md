# Remote Shuffle

## [Cloud Shuffle Service(CSS)](https://github.com/bytedance/CloudShuffleService)

> A general purpose remote shuffle solution for compute engines, including Spark/Flink/MapReduce.

**计算和中间Shuffle数据分离**

### 架构

计算框架将shuffle数据推送到CSS集群并存放在磁盘或者HDFS中， 之后当需要读取数据时，再通过CSS集群的接口**拉取相关**的shuffle数据。

- CSS Worker

  负责把来自Map Task发送的数据存储到内存中并最终异步的把数据刷写到文件系统中。当Reduce Task开始时可以从Worker拉取之前存储的数据。

- CSS Master

  负责协调application的整个shuffle过程，需要被整合进application的运行过程中，例如跑在Spark的driver中。 CSS Master会从ZooKeeper中获取worker的列表并且分配合适的worker给application提供shuffle服务，并且跟进所有Map Task完成的进度， 当Map Task运行完成的时候会通知所有worker把所有缓存中的数据持久化到磁盘并关闭文件。

- CSS Client

  当运行Map或者reduce task时，负责从所有worker推送或者拉取shuffle数据

<img src="pics/css-arch.png" alt="CSS Architecture" style="zoom: 50%;" />



### TODO(https://mp.weixin.qq.com/s/_dWEfuwbJEnHFxsmRY0cGA)



## Apache Uniffle（incubating）

> Apache Uniffle is a unified remote shuffle service for compute engines.

### TODO(https://uniffle.apache.org/zh-CN/)



## Apache Celeborn（incubating）

> Apache Celeborn is an elastic and high-performance service for shuffle and spilled data.
## [Apache Celeborn（incubating）](https://celeborn.apache.org/)

> an intermediate data service for Big Data compute engines (i.e. ETL, OLAP and Streaming engines) to boost performance, stability, and flexibility. Intermediate data typically include shuffle and spilled data.

### [TODO](https://mp.weixin.qq.com/s?__biz=MzU1NTMyOTI4Mw==&mid=2247672756&idx=2&sn=e8569ae85170af0dabe8a8d436d76622&chksm=fbd991d8ccae18ce928e114f38a4966ca2d5784637bacf5eae82f60e360bad3025eb9c4b619d&mpshare=1&scene=24&srcid=0805fLa1ZYiTqF1DExU9gAc9&sharer_sharetime=1691245210118&sharer_shareid=6a353950415186d8ef56f25fc0280325&key=9a1ceb7dc99e5b198cb5f454f2b3ff2a06926551fb8d0400635efe67430c1a6048e7fde1dddd6e25af572edecbbf29f0f1867c4b6dfc8b93ca2de99ff288aada785937575e226e8bf3ac4714c62cc30036efebb9fa8ec6102eeb245d4ad0814a65b14430f8dc394238dce41098118e089d2b603fed746a3688b104b4155f1cfe&ascene=0&uin=NTAyNzY0MDQw&devicetype=Windows+11+x64&version=6308011a&lang=zh_CN&countrycode=CN&exportkey=n_ChQIAhIQRx5BDWw9N%2FbntftTILqoahLYAQIE97dBBAEAAAAAADHGKsAuwR4AAAAOpnltbLcz9gKNyK89dVj0%2FA8ZTf3HLdipPL8JwpgjXtchLK2pv%2FL0Hwecxpmb3PM5unrF4Wv2jUEBXGzz2sMHwHb6La%2BMlEDc7u1dcW9k8UDBqMxp%2FcLe3%2Fi42%2FwUeSGbrWjM%2FE7suSa8jUWLYeel%2FS2%2BOJr49x%2FaahACp4AtqIjuTAPN7a3YZr4drZFalddyFYLBUnosTmjZEcnqUzQr2Oz3rp2%2FxZz8g9Iy1PtPR6ZcHh1yj2egZSg2RHgIoFPAAw%3D%3D&acctmode=0&pass_ticket=X73y9GvEUgJ33TPbvVl1QuLJNDChh%2Bow3ygY4nutkd9DgLNn2hv44wnTsPVGmRY3&wx_header=1&fontgear=2)

