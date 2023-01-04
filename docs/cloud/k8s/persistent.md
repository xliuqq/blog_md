# 生产实践

## 持久化

### Logs持久化

1. 应用日志输出到 stdout/stderr，在宿主机部署 logging-agent 集中处理日志；
2. sidecar形式直接将日志发送到指定好的日志的存储后端；

### Event 持久化

> Event作为kubernetes的一个对象资源，记录了集群运行所遇到的各种大事件,有助于排错，但大量的事件如果都存储在etcd中，会带来较大的性能与容量压力，所以 **etcd 中默认只保存最近1小时**的。

k8s集群规模的增加，集群内的object数量也与日俱增，那么events的数量也会伴随其大量增加，那么当用户请求这些events的时候apiserver的负载压力就会增加，很可能造成 apiserver 处理请求延迟，可以将 events 持久化，通过 webhook 拦截用户请求。

#### [eventrouter](https://github.com/openshift/eventrouter)

> A **simple** introspective kubernetes service that forwards events to a specified sink.

将 `ADD`或者 `Update`的事件，以及对应的 `newEvent`和 `oldEvent`，发送给Sink。

- 通过 `informers.NewSharedInformerFactory`进行监听和处理；
- 统计 `Normal`，`WARNING`相同的次数，给prometheus使用；

#### [kubewatch](https://github.com/robusta-dev/kubewatch)

> **kubewatch** is a Kubernetes watcher that currently publishes notification to available collaboration hubs/notification channels. Run it in your k8s cluster, and you will get event notifications through webhooks.

supported webhooks:

- slack, slackwebhook, hipchat, mattermost, flock, cloudevent
- **webhook**
- smtp

可以配置监听不同的资源，

```yaml
resource:
  deployment: false
  replicationcontroller: false
  replicaset: false
  daemonset: false
  services: true
  pod: true
  job: false
  node: false
  clusterrole: false
  clusterrolebinding: false
  serviceaccount: false
  persistentvolume: false
  namespace: false
  secret: false
  configmap: false
  ingress: false
```

[kube-eventer](https://github.com/AliyunContainerService/kube-eventer)

> kube-eventer emit kubernetes events to sinks.

支持的Sink

| Sink Name                                                                                                      | Description                       |
| -------------------------------------------------------------------------------------------------------------- | --------------------------------- |
| [dingtalk](https://github.com/AliyunContainerService/kube-eventer/blob/master/docs/en/dingtalk-sink.md)           | sink to dingtalk bot              |
| [sls](https://github.com/AliyunContainerService/kube-eventer/blob/master/docs/en/sls-sink.md)                     | sink to alibaba cloud sls service |
| [elasticsearch](https://github.com/AliyunContainerService/kube-eventer/blob/master/docs/en/elasticsearch-sink.md) | sink to elasticsearch             |
| [honeycomb](https://github.com/AliyunContainerService/kube-eventer/blob/master/docs/en/honeycomb-sink.md)         | sink to honeycomb                 |
| [influxdb](https://github.com/AliyunContainerService/kube-eventer/blob/master/docs/en/influxdb-sink.md)           | sink to influxdb                  |
| [kafka](https://github.com/AliyunContainerService/kube-eventer/blob/master/docs/en/kafka-sink.md)                 | sink to kafka                     |
| [mysql](https://github.com/AliyunContainerService/kube-eventer/blob/master/docs/en/mysql-sink.md)                 | sink to mysql database            |
| [wechat](https://github.com/AliyunContainerService/kube-eventer/blob/master/docs/en/wechat-sink.md)               | sink to wechat                    |
| [webhook](https://github.com/AliyunContainerService/kube-eventer/blob/master/docs/en/webhook-sink.md)             | sink to webhook                   |
| [mongodb](https://github.com/AliyunContainerService/kube-eventer/blob/master/docs/en/mongodb-sink.md)             | sink to mongodb                   |

#### 如何获取特定资源的事件

汇总信息：`Age`字段的信息，重复处理？。

```ruby
Events:
  Type     Reason   Age                  From          Message
  ----     ------   ----                 ----          -------
  Normal   Pulled   83s (x5 over 5h36m)  kubelet, t01  Container image "nginx:1.10" already present on machine
  Normal   Created  82s (x5 over 5h36m)  kubelet, t01  Created container nginxtest
  Normal   Started  82s (x5 over 5h36m)  kubelet, t01  Started container nginxtest
```
