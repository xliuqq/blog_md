[toc]

# 数据

DataSet定义数据如何产生以及数据集的大小；

Sampler根据DataSet的长度，产生数据集的索引；

DataLoader根据Sampler返回的索引，去DataSet中获取对应的数据。

## Dataset（接口）

```python
class Dataset {}
    """
    获取index所在的数据，如果带标签，以元组的形式返回 data, label
    """
    def __getitem__(self, index):
        pass
    
    """
    返回数据集的大小
    """
    def __len__(self):
        pass
}
```



## DataLoader（类)

具体类，接收Dataset实现类和Sampler实现类，在Epoch内，根据



## Sampler(接口)

通过迭代器的形式使用，每次返回一组Dataset的索引。

```python
class Sampler(object):
    r"""Base class for all Samplers.

    Every Sampler subclass has to provide an :meth:`__iter__` method, providing a
    way to iterate over indices of dataset elements, and a :meth:`__len__` method
    that returns the length of the returned iterators.

    .. note:: The :meth:`__len__` method isn't strictly required by
              :class:`~torch.utils.data.DataLoader`, but is expected in any
              calculation involving the length of a :class:`~torch.utils.data.DataLoader`.
    """

    def __init__(self, data_source):
        pass

    def __iter__(self):
        raise NotImplementedError
```

## 使用

```python
import torch
from torch.utils.data import Dataset, DataLoader,RandomSampler
import torch.optim as optim
import numpy as np

class CustomizeDataSet(DataSet):
    def __init__(self, len):
        self.len = len
        
    def __getitem__(self, index):
        print("Get index: ", index)
        return np.array([1,2,3,4]), np.int(index)
        
    def __len__(self):
        return self.len
    
dataset = CustomizeDataSet(100)

train_loader = DataLoader(dataset=dataset,
                          batch_size=batch_size,
                          sampler=RandomSampler(dataset))

for batch_idx, (data, target) in enumerate(train_loader):
    print("batch_idx:", batch_idx, ", target:", target)
```