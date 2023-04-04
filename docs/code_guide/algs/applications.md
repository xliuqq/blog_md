# 算法应用

## Tweet 关注事件推送

### 场景描述

Tweet的两个典型的业务场景：

- **发布Tweet消息**：用户可以快速推送新消息到所有关注者，平均大约4.6k request/sec，峰值约**12k request/sec**；
- **主页时间线（Home timeline）浏览**：平均**300K request/sec** 查看关注对象的最新消息；



挑战点：巨大的扇出结构

- 每个用户会关注很多人，也会被很多人圈粉。



思路1：（拉取）

- 将发送的新的 tweet 插入到全局的 tweet 集合。当用户查看时间线时，首先查找所有的关注对象，列出这些人的 tweet，最后一时间为序将进行排序合并。

  ```sql
  # SQL code
  SELECT tweet.*, user.* FROM tweets JOIN users ON tweets.sender_id = users.id
  JOIN follows ON follows.followee_id = users.id
  WHERE follows.follower_id = current_user
  ```

<img src="pics/ch01-fig02.png" alt="twitter-table.png" style="zoom: 25%;" />

问题：

- 用户只有查看时才会去拉取，但是每次查看时现去拉取，呈现速度较慢；



思路2：（推送）

- 对每个用户的时间线维护一个缓存，类似每个用户一个 tweet 邮箱。当用户推送新 tweet 时，查询其关注者，将 tweet 插入到每个关注着的时间线缓存中。因为之前预先将结果取出，之后访问时间线性能非常快。
- 基于实践：时间线浏览 tweet 的压力几乎比发布 tweet 高出两个数量级。因此发布时多完成一些事情。

问题：

- 写放大：粉丝多的人发的一个 tweet 会导致 3000 万笔写入，对于响应性能/存储是挑战；



关键点：每个**用户关注者的分布情况**（还可以结合用户使用 Twitter 的频率进行加权）是扩展性的关键因素。

- 大多数用户的 tweet 在发布时继续以一对多写入时间线；
- 少数具有超多关注者（如名人）的用户，采用方案一，推文被单独提取，在读取时才与用户时间线主表合并。