# 基于浏览器使用CentOS

## 1. 拉取基础CentOS镜像

```bash
# 该镜像的yum源为阿里云
docker pull centos:7.9.2009
```

## 2. docker启动镜像

```bash
docker run -itd --privileged --name centos7.9_vnc -p 15901:5901 -p 16901:6901 centos:7.9.2009 /usr/sbin/init
```

## 3. docker进入容器进行vnc/novnc安装配置

```bash
docker exec -it centos7.9_vnc bash
```

## 4. 设置root密码(root)

此密码即为可视化终端root用户密码

```bash
echo -e "root\nroot" | passwd
```

## ~~5. GNOME桌面(体积过大且需要init权限，改为xfce实现)~~

- 安装 tigervnc-server 并为 root 开启一个 vnc 桌面；
- 请求输入 vnc 密码；
- 安装 GNOME 并设置开机进行图形界面；
- 关闭 selinux 与关闭防火墙；
- 安装 noVNC 于根目录；
- 设置 root 的 VNC 与 noVNC 为开机启动。

```bash
# 下载
curl -L https://gitee.com/panchongwen/my_scripts/raw/main/linux/centos7_vnc_install.sh -o centos7_vnc_install.sh
# 执行安装
bash ./centos7_vnc_install.sh
```

- 在安装完 tigervnc-server 后会让你设置 vnc 密码，输入一个大于六位的密码即可
- 继续会询会是否需要设置一个只可查看不可操作的 view-only 密码，如果不需要，输入 n 回车

## 5. 安装xfce桌面，VNC与noVNC

定制版本，将定制好的文件copy至镜像指定位置

```bash
# 换源
yum install -y  wget
mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.bk
cd /etc/yum.repos.d
wget -nc http://mirrors.aliyun.com/repo/Centos-7.repo
mv Centos-7.repo CentOS-Base.repo
yum clean all
yum list
yum makecache
yum install -y "tigervnc-server"

# 设置密码： root/root，不设置viewonly密码
vncpasswd

# 安装xfce
yum install epel-release
yum groupinstall xfce

# 安装中文字体
yum -y groupinstall Fonts

# 安装火狐浏览器
yum -y install firefox

# 安装python3及numpy
yum -y install python3
pip3 install numpy -i https://pypi.douban.com/simple/
```

### 5.1 设置xface随vncserver启动

- 先启动vncsercver生成配置文件： `/root/.vnc/xstartup`
- 修改`xstartup`

```shell
#!/bin/sh

unset SESSION_MANAGER
unset DBUS_SESSION_BUS_ADDRESS
exec startxfce4
#/etc/X11/xinit/xinitrc
# Assume either Gnome or KDE will be started by default when installed
# We want to kill the session automatically in this case when user logs out. In case you modify
# /etc/X11/xinit/Xclients or ~/.Xclients yourself to achieve a different result, then you should
# be responsible to modify below code to avoid that your session will be automatically killed
if [ -e /usr/bin/gnome-session -o -e /usr/bin/startkde ]; then
	vncserver -kill $DISPLAY
fi
```



## 6. 访问noVNC

- 利用 noVNC 直接在浏览器访问 6901 端口映射出来的端口，密码为设置的密码；

## ~~7. 将vnc服务添加到后台启动(废弃)~~

```yaml
systemctl start vncserver@:1.service && systemctl enable vncserver@:1.service # 启动vnc并加入开机自启
```

## 7. 启动vnc及novnc服务(非特权模式)

```bash
vncserver -geometry 1920x1080


nohup /noVNC/./utils/launch.sh --vnc 0.0.0.0:5901 1>/dev/null 2>&1
```

## 8. 将noVNC服务启动命令加到k8s的yaml中

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: hc-terminal-pod
  namespace: ai-education
  labels:
    app: hc-terminal-pod-label
spec:
  containers:
  - name: hc-terminal-container1
    securityContext:
      privileged: true
    image: 172.16.2.131:30002/starfish/hc-terminal:v3.0               #ts-jupyter是包含hc-cpu-jupyterflow+keras+jupyter的镜像的名字
    imagePullPolicy: IfNotPresent
    resources:
      requests:
        memory: 100Mi
      limits:
        memory: 2000Mi
      requests:
        cpu: "1"
      limits:
        cpu: "1"
    ports:
    - name: http
      containerPort: 6901
    env:
      - name: WEBSOCK_PATH
        value: "001"
    command: ["/bin/bash"]                        #容器启动命令，进入容器的shell终端
    args: ["-c","su root -s /bin/bash  /noVNC/app/sed.sh"]  #启动noVnc
---
apiVersion: v1
kind: Service
metadata:
  name: hc-terminal-service
  namespace: ai-education
spec:
  ports:
  - name: http
    port: 6901                           #jupyter notebook对外暴露服务的端口号
    protocol: TCP
  selector:
    app: hc-terminal-pod-label
  type: NodePort
```

## 9. 打包当前容器为最终可视化终端镜像

```yaml
docker commit centos7.9_vnc hc-terminal:v3.0
```