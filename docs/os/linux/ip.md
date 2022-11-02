# Network

## IP命令

### link 

```shell
# 启动回环设备
$ ip link set dev lo up
```

### netns

```shell
# 创建network namespace
$ ip netns add nstest

# 列出存在的network namespace
$ ip netns list

# 删除network namespace
$ ip netns delete nstest

# 在network namespace中执行命令，形式 ip netns exec <network namespace> <command>
$ ip netns exec nstest ip addr
```

### 示例

新建network namespace，创建veth-pair，分配IP，并与主机互通。

```shell
# 创建 nstest network namespace
$ ip netns add nstest

# 创建network namespace时，默认创建一个回环设备（loopback interface : lo），默认不启动
$ ip netns exec nstest ip link set dev lo up

# 主机上创建veth-pair两张虚拟网卡 veth-a 和 veth-b
$ ip link add veth-a type veth peer name veth-b

# 将veth-b设备添加到nstest，veth-a则留在主机
$ ip link set veth-b netns nstest

# 查看nstest应该具备lo和veth-b两张网卡
$ ip netns exec nstest ip link

# 为主机veth-a网卡分配IP并启动网卡
$ ip addr add 10.0.0.1/24 dev veth-a
$ ip link set dev veth-a up

# 为nstest的veth-b网卡分配IP并启动网卡
$ ip netns exec nstest ip addr add 10.0.0.2/24 dev veth-b
$ ip netns exec nstest ip link set dev veth-b up

# 为网卡配置好IP后，在各自的network namespace中会生成路由，查看
# 主机，表明目的地址10.0.0.0/24的IP包从veth-a网卡发出
$ ip route
10.0.0.0/24 dev veth-a proto kernel scope link src 10.0.0.1 
# nstest，表明目的地址10.0.0.0/24的IP包从veth-b网卡发出
$ ip netns exec nstest ip route
10.0.0.0/24 dev veth-b proto kernel scope link src 10.0.0.2

# 主机veth-a网卡ping nstest的veth-b网卡
$ ping 10.0.0.2

# nstest的veth-b网卡ping主机veth-a网卡
$ ip netns exec nstest ping 10.0.0.1
```

## Brctl



## [pipework(Linux Container)](https://github.com/jpetazzo/pipework)

> Software-Defined Networking tools for LXC (LinuX Containers)