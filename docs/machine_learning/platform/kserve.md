# 模型服务KServe

> https://github.com/kserve/kserve/blob/master/install/v0.9.0/kserve-runtimes.yaml
>
> 针对不同类型的ML 框架，提供不同的服务镜像。

## 架构



### 控制平面

IngressClass 通过 HEADER 控制路由？



### 数据平面



## 安装



## 协议

### prediction v1 protocol



### prediction v2 protocol

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
