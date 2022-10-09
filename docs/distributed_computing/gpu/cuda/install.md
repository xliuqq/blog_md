# 安装

## Cuda

官网：https://developer.nvidia.com/cuda-downloads 

在 https://docs.nvidia.com/cuda/cuda-toolkit-release-notes/index.html 这里可以查询到我们应该下载哪个版本



## CuDNN 

官网 https://developer.nvidia.com/rdp/cudnn-download



**如果使用 Conda 安装，可以安装对应的Cuda。原生安装的cuda肯定是 conda-cudatoolkit 的超集**

```shell
# 添加源
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main

conda install cudatoolkit=10.1
conda install cudnn=7.0.5
```



## NCCL

GPU操作（AllReduce等）的库，nccl 安装

```shell
# conda 安装
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/conda-forge
conda install nccl=2.5
# 系统安装
# https://docs.nvidia.com/deeplearning/nccl/install-guide/index.html
```



