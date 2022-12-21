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
# 启动 nstest 的 lo
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

```shell
# 查看所有的网桥以及对应的网卡信息
$ brctl show
bridge name	    bridge id		STP enabled	    interfaces	
```



## [pipework(Linux Container)](https://github.com/jpetazzo/pipework)

> Software-Defined Networking tools for LXC (LinuX Containers)



## TCP Dump

```shell
tcpdump [ -DenNqvX ] [ -c count ] [ -F file ] [ -i interface ] [ -r file ]
        [ -s snaplen ] [ -w file ] [ expression ]
```

**抓包选项**

-c：指定要抓取的包数量。

**-i interface**：指定tcpdump需要监听的接口。默认会抓取第一个网络接口。

-n：对地址以数字方式显式，否则显式为主机名，也就是说-n选项不做主机名解析。

**-nn**：除了-n的作用外，还把端口显示为数值，否则显示端口服务名。

**-P**：指定要抓取的包是流入还是流出的包。可以给定的值为"in"、"out"和"inout"，默认为"inout"。

**-s len**：设置tcpdump的数据包抓取长度为len，如果不设置默认将会是65535字节。对于要抓取的数据包较大时，长度设置不够可能会产生包截断，若出现包截断，输出行中会出现"[|proto]"的标志(proto实际会显示为协议名)。但是抓取len越长，包的处理时间越长，并且会减少tcpdump可缓存的数据包的数量，从而会导致数据包的丢失，所以在能抓取我们想要的包的前提下，抓取长度越小越好。

**输出选项**

-e：输出的每行中都将包括数据链路层头部信息，例如源MAC和目标MAC。

-q：快速打印输出。即打印很少的协议相关信息，从而输出行都比较简短。

-X：输出包的头部数据，会以16进制和ASCII两种方式同时输出。

-XX：输出包的头部数据，会以16进制和ASCII两种方式同时输出，更详细。

-v：当分析和打印的时候，产生详细的输出。

-vv：产生比-v更详细的输出。

-vvv：产生比-vv更详细的输出。

**功能性选项**

-D：列出可用于抓包的接口。将会列出接口的数值编号和接口名，它们都可以用于"-i"后。

-F：从文件中读取抓包的表达式。若使用该选项，则命令行中给定的其他表达式都将失效。

**-w**：将抓包数据输出到文件中而不是标准输出。可以同时配合"-G time"选项使得输出文件每time秒就自动切换到另一个文件。可通过"-r"选项载入这些文件以进行分析和打印。

**-r**：从给定的数据包文件中读取数据。使用"-"表示从标准输入中读取。
