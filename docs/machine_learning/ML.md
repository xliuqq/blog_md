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
- 



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

**具有大量特征的数据集的降维和可视化**。

- 线性算法。 它不能解释特征之间的复杂多项式关系

#### t-SNE（非线性）

t-分布式随机邻域嵌入是一种用于挖掘高维数据的非线性降维算法。 它将多维数据映射到适合于人类观察的两个或多个维度。

- 通过基于具有多个特征的数据点的相似性识别观察到的模式来找到数据中的规律。

- 基于在邻域图上随机游走的概率分布，可以在数据中找到其结构关系。

应用集中在**图像处理，自然语言处理，基因数据以及语音处理**。它还被用于提高心脑扫描图像的分析。

#### PAC与Sklearn示例

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



