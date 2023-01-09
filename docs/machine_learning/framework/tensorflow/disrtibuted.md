[toc]

# 训练策略

Tensorflow分布式训练的支持主要是通过 [tf.distribute.Strategy](https://www.tensorflow.org/beta/guide/distribute_strategy) 来实现，

## CentralStorageStrategy

中央存储策略，参数被统一存在CPU里，然后复制到所有GPU上，优点是GPU负载均衡了，但是一般情况下CPU和GPU通信代价大，不建议使用。



## MirroredStrategy

- 支持**多张GPU在同一个机器上**的同步训练方法

每个GPU上保存一份模型副本，模型中的每个变量都镜像在所有副本中。这些变量一起形成一个名为`MirroredVariable`的概念变量。通过apply相同的更新，这些变量保持彼此同步。

每个显卡会收到tf.data.Dataset传来的数据，独立计算梯度，然后采用all-reduce的方法进行同步更新。多个显卡在通信时默认使用Nvidia NCCL进行。

```python
# 自定义GPU设备号
mirrored_strategy = tf.distribute.MirroredStrategy(devices=["/gpu:0", "/gpu:1"])
```

all-reduce实现：

- `tf.distribute.CrossDeviceOps`
- `tf.distribute.HierarchicalCopyAllReduce`
- `tf.distribute.ReductionToOneDevice`
- **`tf.distribute.NcclAllReduce` (default)**



## ParameterServerStrategy 

Tensorflow最初的分布式训练方法，使用**PS**进行多机器分布式训练



## MultiWorkerMirroredStrategy

  **多机器同步训练**，在每一个**device**上存储一份模型的备份。

```python
# Demo示例
import tensorflow as tf
import tensorflow_datasets as tfds
import os
import json

num_epochs = 5
batch_size_per_replica = 64
learning_rate = 0.001

# 核心：训练集群的配置信息
os.environ['TF_CONFIG'] = json.dumps({
    'cluster': {
        'worker': ["localhost:20000", "localhost:20001"]
    },
    'task': {'type': 'worker', 'index': 0}
})
strategy = tf.distribute.experimental.MultiWorkerMirroredStrategy()
num_workers = 2
batch_size = batch_size_per_replica * num_workers

def resize(image, label):
    image = tf.image.resize(image, [224, 224]) / 255.0
    return image, label

dataset = tfds.load("cats_vs_dogs", split=tfds.Split.TRAIN, as_supervised=True)
dataset = dataset.map(resize).shuffle(1024).batch(batch_size)

with strategy.scope():
    model = tf.keras.applications.MobileNetV2()
    model.compile(
        optimizer=tf.keras.optimizers.Adam(learning_rate=learning_rate),
        loss=tf.keras.losses.sparse_categorical_crossentropy,
        metrics=[tf.keras.metrics.sparse_categorical_accuracy]
    )

model.fit(dataset, epochs=num_epochs)
```

