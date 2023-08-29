# Mqtt



## 使用

> MqttClient VS MqttClient
>
> - The Async version will connect and publish without blocking；
> - The message processing in CallBack or Listener is **synchronous**；

`maven`

```xml
<dependency>
    <groupId>org.eclipse.paho</groupId>
    <artifactId>org.eclipse.paho.client.mqttv3</artifactId>
    <version>1.2.5</version>
</dependency>
```



订阅

```java
MqttConnectOptions connOpt = new MqttConnectOptions();
connOpt.setCleanSession(true);
connOpt.setKeepAliveInterval(30);
String serverUri = "tcp://iot.eclipse.org:1883";
String clientId = UUID.randomUUID().toString();

MqttClient myClient = new MqttClient(serverUri, clientId);
myClient.connect(connOpt);
myClient.subscribe("topic/foo");
myClient.setCallback(new FooCallbackListener());

```



对于订阅消息两种处理方式：

```java
// default，处理 messageArrived 以及 connectionLost 等
void setCallback(MqttCallback callback);

// 针对每个主题，只处理 messageArrived
void subscribe(String topicFilter, int qos, IMqttMessageListener messageListener) throws MqttException;
```



源码：`CommsCallback.java`

```java
protected boolean deliverMessage(String topicName, int messageId, MqttMessage aMessage) throws Exception
{		
    boolean delivered = false;

    Enumeration<String> keys = callbacks.keys();
    while (keys.hasMoreElements()) {
        String topicFilter = (String)keys.nextElement();
        // callback may already have been removed in the meantime, so a null check is necessary
        IMqttMessageListener callback = callbacks.get(topicFilter);
        if(callback == null) {
            continue;
        }
        if (MqttTopic.isMatched(topicFilter, topicName)) {
            aMessage.setId(messageId);
            ((IMqttMessageListener)callback).messageArrived(topicName, aMessage);
            delivered = true;
        }
    }

    /* if the message hasn't been delivered to a per subscription handler, give it to the default handler */
    if (mqttCallback != null && !delivered) {
        aMessage.setId(messageId);
        mqttCallback.messageArrived(topicName, aMessage);
        delivered = true;
    }

    return delivered;
}
```



