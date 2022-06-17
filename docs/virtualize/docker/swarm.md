# Swarm

## service

Swarm services使用声明性模型（类比 k8s service），意味着你可以定义服务的理想状态，并依靠Docker保持状态。状态信息如下：

- 运行服务容器所依赖的镜像名称和tag
- 该服务包含多少容器
- 是否有任何端口暴露在swarm集群的客户端上
- 当docker服务启动时，service是否自动启动
- 重新启动服务时发生的特定行为（例如是否使用回滚重新启动）
- 服务可以运行的节点的特性（如资源约束和位置偏好）



`docker service create`：

- `--detach, -d`：Exit immediately instead of waiting for the service to converge；
- `--mode`：Service mode (replicated, global, replicated-job, or global-job)；
- `--network`：Attach a service to an existing network；



**docker-compose.yml**