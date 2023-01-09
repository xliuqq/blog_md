[toc]

# 训练

## 单机多卡模型并行

不同的层指定不同的gpu设备



## 单机多卡数据并行

nn.DataParallel，**单节点、单进程、多线程，不支持模型并行**。

基于Parameter server的算法，即使在单台机器上，由于线程之间的GIL竞争、每个迭代的复制模型以及分散输入和收集输出所带来的额外开销，DataParallel通常比DistributedDataParallel慢。



## 分布式数据并行

**nn.parallel.DistributedDataParallel** （DDP），多进程、**单节点/多节点**。

Pytorch 中通过 **torch.distributed** 包提供分布式支持，**支持数据并行和模型并行**，采用**RingAllReduce**

**当DDP与模型并行相结合时，每个DDP进程将使用模型并行，所有进程共同使用数据并行。**

- 用 `NCCL` 进行分布式 `GPU` 训练
- 用 `Gloo` 进行分布式 `CPU` 训练

**数据划分：DDP并不会自动shard数据**

- 如果自己写数据流，得根据**`torch.distributed.get_rank()`**去shard数据，获取自己应用的一份；
- 如果用Dataset API，则需要在定义Dataloader的时候用**`DistributedSampler` **去shard：

```python
import torch
from torch import distributed
import torch.nn as nn
from torch.nn import functional as F
from torch.utils.data import Dataset, DataLoader
import torch.optim as optim
from torch.utils.data.distributed import DistributedSampler
import os

# 1) 初始化
# init_method有三种方式：1）TCP; 2）File System; 3) Env, default
torch.distributed.init_process_group(backend="gloo", init_method=None)

input_size = 28 * 28
batch_size = 30
data_size = 900
num_epochs = 10

# 2） 配置每个进程的gpu
local_rank = torch.distributed.get_rank()
# torch.cuda.set_device(local_rank)

pid = os.getpid()

# device = torch.device("cuda", local_rank)

class RandomDataset(Dataset):
    def __init__(self, input_size, length):
        self.len = length
        # 生成 length 行， input_size列的数组
        self.data = torch.randn(length, input_size)
        self.ydata = torch.randint(0, 10, [length, 1])

    def __getitem__(self, index):
        # print(pid, "---Get index: ", index, "value is", self.ydata[index])
        return self.data[index], self.ydata[index]

    def __len__(self):
        return self.len


dataset = RandomDataset(input_size, data_size)
# 3）使用DistributedSampler
train_loader = DataLoader(dataset=dataset,
                          batch_size=batch_size,
                          sampler=DistributedSampler(dataset))


class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.conv1 = nn.Conv2d(1, 10, kernel_size=5)
        self.conv2 = nn.Conv2d(10, 20, kernel_size=5)
        self.conv2_drop = nn.Dropout2d()
        self.fc1 = nn.Linear(320, 50)
        self.fc2 = nn.Linear(50, 10)

    def forward(self, x):
        # (10, 14, 14)
        x = F.relu(F.max_pool2d(self.conv1(x), 2))
        # (20, 7, 7)
        x = F.relu(F.max_pool2d(self.conv2_drop(self.conv2(x)), 2))
        x = x.view(-1, 320)
        x = F.relu(self.fc1(x))
        x = F.dropout(x, training=self.training)
        x = self.fc2(x)
        return x


# 4) 封装之前要把模型移到对应的gpu
model = Net()
# model.to(device)

optimizer = optim.SGD(model.parameters(), lr=0.01, momentum=0.5)
# 5) 封装,把原有的model变为DDP模式的model
model = torch.nn.parallel.DistributedDataParallel(model)

loss_func = nn.CrossEntropyLoss()  # the target label is not one-hotted

for epoch in range(num_epochs):
    # 新增2：设置sampler的epoch，DistributedSampler每个epoch对于Shuffle生成不同的随机分配
    train_loader.sampler.set_epoch(epoch)
    for batch_idx, (data, target) in enumerate(train_loader):
        # data, target 一= data.cuda(), target.cuda()
        optimizer.zero_grad()
        # print("Get data is ", data.shape)
        data = data.reshape([-1, 1, 28, 28])
        output = model(data)
        # print("Get Out is ", output.shape, "-target shape:", target.shape)
        # 目标必须是1D数组
        loss = loss_func(output, target.flatten())
        loss.backward()
        optimizer.step()
        print(pid, '---Train Epoch-batch: {}-{}\tLoss: {:.6f}'.format(epoch, batch_idx, loss.item()))

# 保存的是model.module,在进程0上保存一次
if torch.distributed.get_rank() == 0:
    torch.save(model.module, "/root/tf/pytorch/model.ckpt")

```

**init_method**有三种方式：

1）TCP

2）File System

3）Env, default

启动命令

```shell
# 机器1
CUDA_VISIBLE_DEVICES="4,5,6,7" python -m torch.distributed.launch --nnodes=2 --node_rank=0 --nproc_per_node=4 \
  --master_addr="172.16.1.217" --master_port=1234 main.py
# 机器2：
CUDA_VISIBLE_DEVICES="4,5,6,7" python -m torch.distributed.launch --nnodes=2 --node_rank=1 --nproc_per_node=4 \
  --master_addr="172.16.1.217" --master_port=1234 main.py
```

或者 `torch.multiprocessing.spawn`（单机多卡的形式）

```python
def demo_fn(rank, world_size):
    dist.init_process_group("nccl", rank=rank, world_size=world_size)
    # lots of code.
    ...

def run_demo(demo_fn, world_size):
    mp.spawn(demo_fn,
             args=(world_size,),
             nprocs=world_size,
             join=True)
```



## RPC API

RPC API可以实现 PS。

如果模型需要跨多个机器，使用**RPC API** 获取更通用的分布式训练支持。