# 部署

在生产环境中，会把vue项目打包成静态资源，然后放到nginx中提供访问。



## 多前端项目部署

### vue的基础路径

- **前端vue的地址，加统一前缀**；

  - vue-router的basepath字段；

- **禁止通过windows.location_href 等强行改变浏览器的路由**；

  - 通过vue-router的push等方式改变路由

  

### nginx的路径区分（mode="history"）

请求 127.0.0.1/images/test.gif 会依次查找 

1. 文件/opt/html/images/test.gif   
2. 文件夹 /opt/html/images/test.gif/下的index文件  
3.  请求127.0.0.1/images/index.html

```conf
location /images {
    root /opt/html/;
    index  index.html;
    # web路由刷新时，如/images/liuyifei/show，会根据相对路径去搜索/images/liuyifei/show文件，然后搜索不到，会继续搜索/images/index.html
    # vue路由必须先加载 index.html 或者XX.js 才能识别到路由，故直接刷新页面会出现404，这时候最好到做法，是使用try_files 进行尝试，如果获取不到资源，加载index.html 
    try_files $uri  $uri/  /images/index.html; 
}
```

