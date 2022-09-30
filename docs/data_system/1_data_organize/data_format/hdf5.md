# HDF5

**HDF 是 Hierarchical Data Format（分层数据格式）的缩写。**

一个HDF5文件就是一个容器，用于储存两类对象：

- datasets，类似于数组的数据集合；

- groups，类似于文件夹的容器，可以储存datasets和其它groups。

Python里面有两个包都支持HDF5的开发，**h5py**和**PyTables**：

>  groups类似于字典（dictionaries），dataset类似于Numpy中的数组（arrays）

pandas包中有DataFrame.to_hdf的功能。

