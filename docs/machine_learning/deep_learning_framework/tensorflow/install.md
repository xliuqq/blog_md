**安装**

https://tensorflow.google.cn/install/source#linux

| 版本             | Python 版本 | 编译器    | 构建工具    | cuDNN | CUDA |
| :--------------- | :---------- | :-------- | :---------- | :---- | :--- |
| tensorflow-2.4.0 | 3.6-3.8     | GCC 7.3.1 | Bazel 3.1.0 | 8.0   | 11.0 |
| tensorflow-2.3.0 | 3.5-3.8     | GCC 7.3.1 | Bazel 3.1.0 | 7.6   | 10.1 |
|                  |             |           |             |       |      |

```shell
conda install tensorflow=2.3.0 cudatoolkit=10.1 cudnn=7.6
```

或者pip安装

```shell
pip install tensorflow
```



**测试**

```python
from tensorflow.python.client import device_lib
print(device_lib.list_local_devices())
```

