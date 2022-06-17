[toc]

# 负载均衡算法

考虑一个负载均衡器（LB）将n个请求随机等概率的发送给n个Server，那么就可以知道Server上最大的可能负载会是多少。

## The Power of Two Random Choices

思想是：

1. 从可用节点列表中做两次随机选择操作，得到节点 A、B
2. **比较 A、B 两个节点，选出负载最低（一般是正在处理的连接数/请求数最少）的节点作为被选中的节点**



问题1： 如果N个Object随机哈希到N个桶，最大链表长度可能是多少呢？

答案： log N / log log N



问题2： 如果每次选择哈希映射的桶时，随机D次挑出D个桶，并从中选择链表长度相对更短的桶插入呢？

答案：log log N / log D + 1



## Small Cache, Big Effect

> Fan B, Lim H, Andersen D G, et al. Small cache, big effect: Provable load balancing for randomly partitioned cluster services[C]//Proceedings of the 2nd ACM Symposium on Cloud Computing. ACM, 2011: 23.

在load balancer中集成一层空间复杂度下界为O(nlogn)的cache*（n为后端节点数！）*来缓存**热点数据**，即可在显著提升load balancer吞吐量的同时，保证到达后端节点的请求负载是均匀分布的。