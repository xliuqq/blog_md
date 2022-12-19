# 数据集

## 人脸关键点检测数据库

现阶段**人脸关键点检测数据库**通常为复杂环境下采集的数据库。

**[LFPW](https://neerajkumar.org/databases/lfpw/)** ：1132 幅训练人脸图像和 300 幅测试人脸图像，大部分为正面人脸图像，每个人脸标定 29 个关键点。

**AFLW** ：25993 幅采集的人脸图像，每个人脸标定 21 个关键点。

**COFW** ：包含 LFPW 训练集中的 845 幅人脸图像以及其他 500 幅遮挡人脸图像，测试集为 507 幅严重遮挡（同时包含姿态和表情的变化）的人脸图像；

**MVFW** ：多视角人脸数据集，包括 2050 幅训练人脸图像和 450 幅测试人脸图像，每个人脸标定 68 个关键点。

**OCFW** ：包含 2951 幅训练人脸图像（均为未遮挡人脸）和 1246 幅测试人脸图像（均为遮挡人脸），每个人脸标定 68 个关键点。

**WFLW**：7500 用于训练，2500 张用于测试，除了98个关键点之外，还有遮挡、姿态、妆容、光照、模糊和表情等信息的标注。

## UCI数据集

http://archive.ics.uci.edu/ml/datasets.php



## LIBSVM

https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/





### 二分类

https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/binary.html

| 数据集名称                              | 行数          | 特征数     |
| --------------------------------------- | ------------- | ---------- |
| avazu                                   | 40,428,967    | 1,000,000  |
| covtype.binary                          | 581,012       | 54         |
| criteo                                  | 45,840,617    | 1,000,000  |
| criteo_tb                               | 4,195,197,692 | 1,000,000  |
| epsilon                                 | 400,000       | 2,000      |
| HIGGS                                   | 11,000,000    | 28         |
| kdd2010 (algebra)                       | 8,407,752     | 20,216,830 |
| kdd2010 (bridge to algebra)             | 19,264,097    | 29,890,095 |
| kdd2010 raw version (bridge to algebra) | 19,264,097    | 1,163,024  |
| kdd2012                                 | 119,705,032   | 54,686,452 |
| SUSY                                    | 5,000,000     | 18         |
| url                                     | 2,396,130     | 3,231,961  |
| webspam                                 | 350,000       | 16,609,143 |

### 多分类

https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/multiclass.html

### 回归

https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/regression.html

### 多标签分类

https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/multilabel.html



## 文本语料库（中文）

### 分词语料库

[SIGHAN - 2nd International Chinese Word Segmentation Bakeoff](https://link.zhihu.com/?target=http%3A//sighan.cs.uchicago.edu/bakeoff2005/)



### 词性标注语料库



### 命名实体识别语料库



### 句法分析语料库



### 文本分类语料库



### 语料库建设

brat(brat rapid annotation tool) 标注软件，支持词性标注、命名实体识别和句法分析等任务。