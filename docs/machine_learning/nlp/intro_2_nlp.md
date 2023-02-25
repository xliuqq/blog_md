# Introduction to Natural Language Processing

统计机器学习方法入门介绍，采用[HaNLP 1.x ](https://github.com/hankcs/HanLP/tree/v1.7.8) 版本。

```xml
<dependency>
    <groupId>com.hankcs</groupId>
    <artifactId>hanlp</artifactId>
    <version>1.7.8</version>
</dependency>
```



## 基础

TODO：自然语言处理的层次（图示）



## 词典分词

分词大致分为：基于词典规则和基于机器学习两种。

> 齐夫定律（Zipf's Law）：在自然语言的语料库里，一个**单词出现的频率与它在频率表里的排名成反比**。

### 切分算法

- 完全切分

  找出一段文本中的所有单词。

- 正向最长匹配

- 逆向最长匹配
- 双向最长匹配：正向和逆向中词数最少的结果，相同则返回单字最少，若还相同返回逆向。
  - 启发：汉语中单字词的数量远小于非单字词。