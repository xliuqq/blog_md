# 机器学习

机器学习算法分成三类：**有监督学习、无监督学习、增强学习**

- 有监督学习需要标识数据（用于训练，即有正例又有负例），无监督学习不需要标识数据，增强学习介于两者之间（有部分标识数据）。



**有监督学习**：

- 决策树
- 朴素贝叶斯分类器
- 最小二乘法
- 逻辑回归
- 支持向量机（SVM）
- 集成学习



**无监督学习**

- 聚类算法：中心聚类、关联聚类、密度聚类、概率聚类、降维、神经网络/深度学习。
- 主成分分析（PCA）：主成分分析是利用正交变换将一些列可能相关数据转换为线性无关数据，找到主成分。
- SVD矩阵分解：PCA实际上就是一个简化版本的SVD分解。
- 独立成分分析(ICA)：发现存在于随机变量下的隐性因素，假设潜在因素属于非高斯分布、并且相互独立，称之为可观测数据的独立成分。



## 分类

### K-近邻算法

优缺点和适用范围
	精度高、对异常值不敏感、无数据输入假定
	时空复杂度高
	数据：数值型和标称型

### 朴素贝叶斯

优缺点和适用范围
	数据少的时候仍然有效，处理多类别分类
	输入数据的准备方式敏感
	数据：标称型（多项式和伯努利（0-1）模型），数值型（高斯模型）	

### Logistic Regression

优缺点和适用范围
	计算代价不高，无需假设数据分布，得到近似概率预测
	容易欠拟合，精度可能不高
	数据：数值型和标称型

### 决策树

优缺点和适用范围	
	计算复杂度不高，中间值缺失不敏感，可以处理不相关特征
	过拟合
	数据：数值型和标称型

## 聚类





## 特征工程

> [Featuretools](https://www.featuretools.com/)是执行自动化特征工程的框架

### 降维

降维是在2维或3维中展现多维数据（具有多个特征的数据，且彼此具有相关性）的技术。

降维算法：

1. 主成分分析（线性）

2. t-SNE（非参数/非线性）

3. 萨蒙映射（非线性）

4. 等距映射（非线性）

5. 局部线性嵌入(非线性)

6. 规范相关分析（非线性）

7. SNE(非线性)

8. 最小方差无偏估计（非线性）

9. 拉普拉斯特征图（非线性）

**除t-SNE之外的大多数非线性技术都不能同时保留数据的局部和全局结构。**

#### 主成分分析（PCA，线性）

具有**大量特征**的数据集的**降维**和**可视化**。

- 线性算法。 它**不能解释特征之间的复杂多项式**关系；
- **当数据集不同维度上的方差分布不均匀的时候**，PCA最有用；

原理：将高维数据集映射到低维空间的同时，尽可能的保留更多变量。



#### t-SNE（非线性）

t-分布式随机邻域嵌入是一种用于挖掘高维数据的非线性降维算法。 它将多维数据映射到适合于人类观察的两个或多个维度。

- 通过基于具有多个特征的数据点的相似性识别观察到的模式来找到数据中的规律。

- 基于在邻域图上随机游走的概率分布，可以在数据中找到其结构关系。

应用集中在**图像处理，自然语言处理，基因数据以及语音处理**。它还被用于提高心脑扫描图像的分析。

#### PCA与Sklearn示例

```python
# importing the required packages
from time import time

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import offsetbox
from sklearn import (manifold, datasets, decomposition)

# Loading and curating the data
digits = datasets.load_digits(n_class=10)
X = digits.data
y = digits.target
n_samples, n_features = X.shape
n_neighbors = 30


# Function to Scale and visualize the embedding vectors
def plot_embedding(X, title=None):
    x_min, x_max = np.min(X, 0), np.max(X, 0)
    X = (X - x_min) / (x_max - x_min)
    plt.figure()
    ax = plt.subplot(111)
    for i in range(X.shape[0]):
        plt.text(X[i, 0], X[i, 1], str(digits.target[i]), color=plt.cm.Set1(y[i] / 10.),
                 fontdict={'weight': 'bold', 'size': 9})
    if hasattr(offsetbox, 'AnnotationBbox'):
        # only print thumbnails with matplotlib> 1.0
        shown_images = np.array([[1., 1.]])  # just something big
    for i in range(digits.data.shape[0]):
        dist = np.sum((X[i] - shown_images) ** 2, 1)
        if np.min(dist) < 4e-3:
            # don't show points that are too close
            continue
        shown_images = np.r_[shown_images, [X[i]]]
        imagebox = offsetbox.AnnotationBbox(offsetbox.OffsetImage(digits.images[i], cmap=plt.cm.gray_r), X[i])
        ax.add_artist(imagebox)
        plt.xticks([]), plt.yticks([])
    if title is not None:
        plt.title(title)


# ----------------------------------------------------------------------
# Plot images of the digits
n_img_per_row = 20
img = np.zeros((10 * n_img_per_row, 10 * n_img_per_row))
for i in range(n_img_per_row):
    ix = 10 * i + 1
    for j in range(n_img_per_row):
        iy = 10 * j + 1
        img[ix:ix + 8, iy:iy + 8] = X[i * n_img_per_row + j].reshape((8, 8))
        plt.imshow(img, cmap=plt.cm.binary)
        plt.xticks([])
        plt.yticks([])
        plt.title('A selection from the 64-dimensional digits dataset')

# Computing PCA
print("Computing PCA projection")
t0 = time()
X_pca = decomposition.TruncatedSVD(n_components=2).fit_transform(X)
plot_embedding(X_pca, "Principal Components projection of the digits (time %.2fs)" % (time() - t0))

# Computing t-SNE
print("Computing t-SNE embedding")
t0 = time()
tsne = manifold.TSNE(n_components=2, init='pca', random_state=0)
X_tsne = tsne.fit_transform(X)
plot_embedding(X_tsne, "t-SNE embedding of the digits (time %.2fs)" % (time() - t0))
plt.show()
```





### 数值归一化

​	欧几里德距离受单位影响，应当先归一化；余弦距离则无需归一化
​	归一到0~1或-1~1之间，即min-max或z-score



### 特征选择

Feature Selection  ( FS ) 

特征选择的的一般流程： 找一个集合，然后针对某个学习算法， 测试效果如何， 一直循环直到找到最优集合为止。

常见的搜索算法：

- 穷举法（Exhaustive）： 暴力穷尽
- 贪心法（Greedy Selection）： 线性时间
- 模拟退火（Simulated Annealing）： 随机尝试找最优
- 基因算法（Genetic Algorithm）： 组合深度优先尝试
- 邻居搜索（Variable Neighbor Search）： 利用相近关系搜索



### 特征抽取

Feature Extraction ( FE )

## 推荐算法

推荐模型会负责收集诸如用户偏好、物品描述这些可用作推荐凭借的数据，据此预测特定用户组可能感兴趣的物品。

**主要的推荐算法系列有四个：**

- 协同过滤（Collaborative Filtering）的推荐算法
- 基于内容过滤（Content-based Filtering）的推荐算法
- 混合型推荐算法
- 流行度推荐算法

### 协同过滤

#### 基于邻域(相似)类型的协同过滤

> 两种方式都能为同一名用户得出不同的推荐结果，尽管两者的输入内容是相同的。

基于**用户**的协同过滤（用户间关系）：**探索对物品拥有相似品味的用户**，并基于彼此喜爱的物品来进行互推。

- 根据用户对物品的评分，**计算用户间的相似性**（如余弦函数），取最相似的用户所阅读的书籍，进行加权对这名用户尚未评分的书籍进行推荐。

基于**物品**的协同过滤（物品间关系）：基于用户喜爱的物品，推荐类似的东西。而这种相似性建立在物品同时出现的基础上，比如购买了x物品的用户也购买了y物品。

- 根据用户此前曾评过分的物品，**计算物品间相似性**，推荐与其最为相似的物品。

优点：

- 简单高效，且生成的推荐结果准确、个性化；

缺点：

- 更容易推荐流行物品（流行度偏好问题），冷启动问题（新用户/新用品），维度爆炸计算量的问题，数据稀疏的问题

#### 基于模型的协同过滤

如**矩阵因子分解，受限玻尔兹曼机，贝叶斯网络**等；

- 矩阵因式分解：其**潜在特性代表用户如何对物品进行评分**。根据用户与物品的潜在表现，预测用户对未评分的物品的喜爱程度。

### 内容过滤

> 假设每个物品都有足够的**描述信息**可作为特征向量（y）（比如标题、年代、描述），而这些特征向量会被用来创建用户偏好模型。

**信息检索**：如tfidf、Okapi BM25等；

**机器学习**：如朴素贝叶斯、SVM、决策树等；

优点：

- 克服流行度偏见，以及新物品的冷启动问题；

缺点：

- 效果不如那些基于使用数据的系统（比如协同过滤算法）；
- 过度专业化的问题（推荐相同类型的商品），无意外之喜；

### 混合算法

> **结合两种甚至更多的基础推荐算法，以创建新混合算法**。（加权或者集成）

**流行度算法**（下载最多等流行度指标）可以引导推荐系统在换到其他更切合用户兴趣点的算法（比如协同过滤算法以及基于内容过滤的算法）前获得足够的活跃度与使用量。流行度模型也可以引入混合算法中，从而解决新用户的冷启动问题。

### 其他算法和实践

还有许多其他算法也可用于加强推荐系统，如：

- 深度学习算法
- 社会化推荐
- 基于机器学习的排序方法
- Multi-armed bandits推荐算法（探索/利用）
- 情景感知推荐（张量分解&分解机）

在实践中：

- 一般如果在推荐模型中使用协同过滤算法，就不会犯太大错误；
- 协同过滤算法似乎比其他算法更优秀，但在冷启动用户及物品方面会有问题，因此通常会使用基于内容的算法作为辅助；
- 选择数据集、处理、后处理、在线模块及用户界面也同样重要；

## 超参调优

神经网络的选择：

- 网络有多深、多宽？
- 每一层是要用什么结构？线性层还是卷积层？
- 层与层之间应该如何连接？
- 应该使用什么样的Activation？
- 应该使用什么样的优化算法？
- 优化算法的初始步长是多少，初始步长在训练过程中应该如何下降？
- 应该使用什么样的初始化？
- 是否需要使用Momentum算法？如果是，具体速率是多少？
- 卷积层里面是否要加入常数项？
- 是否需要使用Dropout？
- 是否需要使用Batch norm？是否需要自动调整Batch norm的参数？
- 是否需要使用Weight decay，Weight decay速度是多少？
- Mini batch的大小是多少？

黑盒优化 （black-box function optimization）:

- 网格搜索 （grid search）：通过遍历给定的参数组合来优化模型表现的方法。网格搜索的问题是很容易发生维度灾难，优点是很容易并行。
- 随机搜索 （random search）：利用随机数求极小点而求得函数近似的最优解的方法。
- 贝叶斯优化：迭代的优化算法，包含两个主要的元素，**输入数据假设的模型**和**一个采集函数**用来来决定下一步要评估哪一个点

### 工具

黑盒优化的工具：

- hyperopt

  hyperopt 是一个Python库，可以用来寻找实数,离散值,条件维度等搜索空间的最佳值

- Google Vizier

  Google的内部的机器学习系统 Google Vizier能够利用迁移学习等技术自动优化其他机器学习系统的超参数

- advisor：https://github.com/tobegit3hub/advisor

  Google Vizier的开源实现

- katib：https://github.com/kubeflow/katib

  基于Kubernetes的超参数优化工具

### Harmonica

> 论文：[ Hyperparameter Optimization: A Spectral Approach (arxiv.org)](https://arxiv.org/abs/1706.00764)
>
> 源码：https://github.com/callowbird/Harmonica

- 适用于离散参数的情况，且离散参数都可以转化为布尔参数，因此只考虑参数是布尔的情况；
- 对于连续参数，可以使用赌博机(Multi-armed Bandit)+最速下降法(Gradient Descent)方法，或者把它们离散化成为离散参数；

拉锁（Lasso）算法：已知***`y=Ax`***，已知向量***y*** 和矩阵***A*** ，想要求***x***。 

- 当 ***A*** 很扁时（行少列多），***x***的维度很高，假设***x***很稀疏，这通过lasso可以求解***x***；

矩阵可以看做是测量矩阵，100行表示尝试100个不同的参数组合，10000列表示每个参数组合可以观察到有10000个特征：

- 认为需要优化的这个参数函数，**本质是一个线性函数**，更加确切地说，是一个稀疏的线性函数；
- 参数函数几乎一定不是**参数**的线性叠加，但是它一定可以写成**某些特征**的线性叠加；

例如：深度神经网络对图像分类可以看做是它的前n-1层对图片的像素进行特征提取，得到了最后一层的特征向量。然后最后一层再做一个线性叠加（linear layer），得到了最后的答案。

- 从这个角度来看，其实神经网络也假设图片分类的函数是一个线性函数，不过线性函数的特征向量是神经网络自己学出来。

> 任何的基于n个布尔参数的参数函数，都可以写成基于2^n个傅里叶基函数（Fourier Basis）的线性叠加。

问题1：维度过大，若n比较大，比如有60，那么2^n显然不可接受

- 低度数傅里叶基（Low degree Fourier Basis），即那些最多只包含d个参数相乘的基函数（特征数降为n^d，d=3,4）

问题2：拉锁算法能够找到***x***的前提是***x***是一个稀疏向量。但是，实际上**x***根本就不是一个稀疏向量*

- 多层Lasso，先跑一次拉锁得到部分重要的特征（5、6个），固定这些特征对剩下的参数**重新进行随机采样，然后跑拉锁**

Harmonica算法流程：

1. 在参数空间中，随机采样（比如）100个点；
2. 对每个点计算**低度数傅里叶基**的特征向量，捕捉参数之间的相关性；
3. 对于计算好的100个特征向量，跑拉锁算法，得到（比如） 5个重要的特征，以及这些特征对应的参数；
4. 固定这些参数的值，得到了新的调参数问题（参数个数减少，搜索空间降低）。回到第一步

Harmonica的几个优点：

- 对参数个数的增长不敏感
- 优化速度极快且容易并行
- 可以自动高效地提取重要的特征



## 元学习 Meta Learning

通过对现有的学习任务之间的性能差异进行系统的观测，然后学习已有的经验和元数据，用于更好的执行新的学习任务。

## AutoML

> Auto-Sklearn
>
> [AutoKeras](https://github.com/keras-team/autokeras)
>
> [TPOT](https://github.com/epistasislab/tpot)
>
> Python auto_ml 

**超参调优**：有**网格搜索、随机梯度下降和贝叶斯超参优化**。

- 网格搜索效率慢，随机梯度局部最优；

**模型选择的同时进行超参调优**：强化学习，经典问题“多摇臂娃娃机”。

**同时选择特征工程、模型选择和超参调优**：Q-Learning。

**元学习**：针对Q-Learning的冷启动问题



### 神经网络架构搜索

> 以Google提出的高效神经网络架构搜索算法ENAS为例，它克服了神经架构搜索算力成本巨大的缺陷，将算力成本减少1000倍以上，仅用一块英伟达GTX 1080Ti显卡，就能在16个小时之内完成架构搜索。

**自动化的神经网络架构搜索NAS**：

- ENAS使用一个RNN（称为controller）决定每个节点的计算类型和选择激活哪些边。ENAS中使用节点数为12的搜索空间，计算类型为tanh，relu，identity，sigmoid四种激活函数，所以搜索空间有 约4^N*N!种约10^15种神经网络架构。
