# PeerJS



## Server

> [peers/peerjs-server: Server for PeerJS (github.com)](https://github.com/peers/peerjs-server)



### 运行

**Native**

```shell
# Install the package globally
$ npm install peer -g
# Run the server
$ peerjs --port 9000 --key peerjs --path /myapp
```

**Docker**

```shell
$ docker run -p 9000:9000 -d peerjs/peerjs-server
```

**Kubernetes**

```shell
$ kubectl run peerjs-server --image=peerjs/peerjs-server --port 9000 --expose -- --port 9000 --path /myapp
```



Server 运行的代码

```javascript
const { PeerServer } = require('peer');

const peerServer = PeerServer({ port: 9000, path: '/myapp' });
```





### 连接

Connecting to the server from client PeerJS

```javascript
<script>
    const peer = new Peer('someid', {
      host: 'localhost',
      port: 9000,
      path: '/myapp'
    });
</script>
```

## Client

### 示例

#### 消息发送

```typescript
// 创建连接，初始会跟 PeerServer 通信
const peer = new Peer({ host: 'localhost', port: 9000, path: '/myapp' });
// 发送
const conn = peer.current.connect(friendId);
conn.on('open', () => {
  console.log('Connected.');
  conn.send({ id, msg: 'Hello, my friend!' });
});

// 接收
peer.current.on('connection', conn => {
  conn.on('data', data => {
    const received = data as SendData;
      console.log(`Data from Peer(id: ${received.id}) => ${received.msg}`);
    });
});
```

#### 共享桌面视频流捕获及传输

**关键代码**

```typescript
// 发送桌面视频流
const sendMediaStream = () => {
  try {
    window.navigator.mediaDevices.getDisplayMedia({ video: true })
      .then(mediaStream => {
        peer.current.call(friendId, mediaStream);
      });
  } catch (e) {
    console.error(e);
    alert('Send failed.');
  }
};

// 接收桌面视频流信息
peer.current.on('call', call => {
  call.answer();
  call.on('stream', remoteStream => {
    if (myVideo.current) {
      myVideo.current.srcObject = remoteStream;
      myVideo.current.play();
    }
  });
});
```