# Conda

conda 是一个包管理器。值得注意的是，**它不仅仅是 Python 的包管理器，而是一个通用的包管理器，当初设计时被用来管理任何语言的包**。在目前来看，conda 环境中所有语言的包管理，都是为了 Python 而服务的。

**Anaconda 是一个 Python 的发行版**，内置了众多 Python 包和附加软件（pydata 生态圈里的软件），所以 Anaconda 自然内置了 conda。

**而 Miniconda 则提供了一个最小的 conda 安装环境**，十分干净轻巧。

conda 和 virtualenv **都可以创建虚拟环境**，进行 Python 运行环境的隔离。

> 由于 **Conda 管理不同版本的 Python，是把 Python 视为包进行管理**的，所以这里指定 Python 版本，相当于指定了初始化 Conda 环境的包



## 源修改

具体可以查看 https://mirrors.tuna.tsinghua.edu.cn/help/anaconda/ 配置更多channel

```shell
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main/
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
```

设置搜索时显示通道地址：

```python
conda config --set show_channel_urls yes
```



## 环境隔离

> - Conda create 创建的环境，会在 Conda 安装目录下进行集中管理

```shell
conda create -n $NAME_OF_ENV python=3.7

conda install package_name

conda env list

conda activate $ENV_NAME

conda deactivate

conda remove -n your_env_name --all
```



