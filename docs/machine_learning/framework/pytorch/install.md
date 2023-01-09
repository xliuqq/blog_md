## 安装

建议采用conda安装

https://pytorch.org/get-started/locally/

```shell
# 添加源
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/pytorch/

# gpu版
conda install pytorch torchvision torchaudio cudatoolkit=10.1 -c pytorch
# cpu版
conda install pytorch torchvision torchaudio cpuonly
```



或者pip安装（手动安装 cuda 和 cudnn？）

```shell
pip install torch===1.5.0 torchvision===0.5.0 -f https://download.pytorch.org/whl/torch_stable.html
```



## 测试

```python
import torch
print(torch.cuda.is_available())

print(torch.backends.cudnn.version())
```

