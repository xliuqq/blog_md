# 模型服务KServe

> https://github.com/kserve/kserve/blob/master/install/v0.9.0/kserve-runtimes.yaml
>
> 针对不同类型的ML 框架，提供不同的服务镜像。

## 架构



### 控制平面

> 负责对`InferenceService`自定义资源进行调谐。

IngressClass 通过 HEADER 控制路由？



**serverless mode**：creates the Knative serverless deployment for predictor, transformer, explainer to enable autoscaling

**raw deployment mode**：creates Kubernetes deployment, service, ingress, HPA.



组件：

- **KServe Controller**: 负责创建 `service, ingress resources, model server container and model agent container`；
- **Ingress Gateway**:  网关，路由外部/内部的请求；

在 Serverless 模式：

- Knative Service Controller
- Knative Activator
- Knative Autoscaler

<img src="pics/kserve_controlplane.png" alt="Architect" style="zoom: 50%;" />

### 数据平面



![Data Plane](pics/kserve_dataplane.jpg)

组件：

- **Component**：Each endpoint is composed of multiple components: "predictor", "explainer", and "transformer". 
- **Predictor**：model server  available at a network endpoint.
- **Explainer**： provides model explanations in addition to predictions.
- **Transformer**：define a pre and post processing step before the prediction and explanation workflows.



#### prediction v1 protocol

| API       | Verb | Path                | Payload                                                      |
| --------- | ---- | ------------------- | ------------------------------------------------------------ |
| Readiness | GET  | /v1/models/         | Response:{"name": , "ready": true/false}                     |
| Predict   | POST | /v1/models/:predict | Request:{"instances": []} Response:{"predictions": []}       |
| Explain   | POST | /v1/models/:explain | Request:{"instances": []} Response:{"predictions": [], "explainations": []} |



#### prediction v2 protocol

> The [*Predict Protocol, version 2*](https://github.com/kserve/kserve/blob/master/docs/predict-api/v2/required_api.md) is a set of HTTP/REST and GRPC APIs for inference / prediction servers.

**Health:**

GET `v2/health/live`

GET `v2/health/ready`

GET `v2/models/${MODEL_NAME}[/versions/${MODEL_VERSION}]/ready`

**Server Metadata:**

GET `v2`

**Model Metadata:**

GET `v2/models/${MODEL_NAME}[/versions/${MODEL_VERSION}]`

**Inference:**

POST `v2/models/${MODEL_NAME}[/versions/${MODEL_VERSION}]/infer`



## 安装

### k8s deployment

#### 创建 `IngressClass`

以 `Nginx`为例，

- 安装 ingress - nginx

```shell
helm upgrade --install ingress-nginx ingress-nginx \
  --repo https://kubernetes.github.io/ingress-nginx \
  --namespace ingress-nginx --create-namespace
```

- ingress - nginx 自带 IngressClass

```yaml
apiVersion: networking.k8s.io/v1
kind: IngressClass
metadata:
  labels:
    app.kubernetes.io/component: controller
    app.kubernetes.io/instance: ingress-nginx
    app.kubernetes.io/name: ingress-nginx
    app.kubernetes.io/part-of: ingress-nginx
    app.kubernetes.io/version: 1.5.1
  name: nginx
spec:
  controller: k8s.io/ingress-nginx
```

#### 安装 CertManager Yaml

```shell
kubectl apply -f https://github.com/cert-manager/cert-manager/releases/download/v1.11.0/cert-manager.yaml
```



#### 安装Kserve Yaml

```shell
kubectl apply -f https://github.com/kserve/kserve/releases/download/v0.10.0/kserve.yaml

kubectl apply -f https://github.com/kserve/kserve/releases/download/v0.10.0/kserve-runtimes.yaml
```

#### 

#### 修改  `inferenceservice-config`

部署模式为 `RawDeployment`

```shell
kubectl patch configmap/inferenceservice-config -n kserve --type=strategic -p '{"data": {"deploy": "{\"defaultDeploymentMode\": \"RawDeployment\"}"}}'
```



ingressClassName 为创建的 IngressClass

```yaml
ingress: |-
{
    "ingressClassName" : "your-ingress-class",
}
```

## 使用

### 模型支持

https://kserve.github.io/website/0.9/modelserving/v1beta1/serving_runtime/



### 单模型部署

**模型服务运行时**：针对不同框架 sklearn, tensorflow, pytorch, mlflow 都提供不同的CRD，运行不同的镜像；

```yaml
apiVersion: "serving.kserve.io/v1beta1"
kind: "InferenceService"
metadata:
  name: "torchscript-cifar"
spec:
  predictor:
    model:
      modelFormat:
        name: tensorflow
      storageUri: "s3://kfserving-examples/models/tensorflow/flowers"
      # 修改运行时镜像的版本号
      runtimeVersion: 2.7.1
```

kubectl apply 后通过 `kubectl get isvc flower-example` 查看状态

```shell
$ kubectl get isvc flower-sample
NAME            URL                                        READY   PREV   LATEST   PREVROLLEDOUTREVISION        LATESTREADYREVISION                     AGE
flower-sample   http://flower-sample.default.example.com   True           100       
```

接口调用

```shell
MODEL_NAME=flower-sample
INPUT_PATH=@./input.json
SERVICE_HOSTNAME=$(kubectl get inferenceservice ${MODEL_NAME} -o jsonpath='{.status.url}' | cut -d "/" -f 3)

curl -v -H "Host: ${SERVICE_HOSTNAME}" http://${INGRESS_HOST}:${INGRESS_PORT}/v1/models/$MODEL_NAME:predict -d $INPUT_PATH
```

其中输入数据格式为：

```json
{  
    "instances":[  
       {  
          "image_bytes":{  
              "b64": ...
           },
           "key":"   1"
       }
    ]
}
```



### 多模型部署

默认模式是'one model, one server' paradigm，当模型过多，会出现：

- `Compute resource limitation`：cpu/gpu 的资源限制
- `Maximum pods limitation`
  - Kubelet has a maximum number of pods per node with the default limit set to [110](https://kubernetes.io/docs/reference/command-line-tools-reference/kubelet/)，不建议超过 100；
- `Maximum IP address limitation`.
  - Each pod in InferenceService needs an independent IP.



### Transformers（TODO）

