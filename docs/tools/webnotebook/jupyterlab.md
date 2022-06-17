# Jupyterlab



JUPYTER_RUNTIME_DIR，JUPYTER_DATA_DIR

## 配置

### Application Directory

stores the main build of JupyterLab with associated data, including extensions built into JupyterLab.

- 默认是`<sys-prefix>/share/jupyter/lab`，其中`<sys-prefix>`是python环境的site路径；
- 可以被`JUPYTERLAB_DIR`覆盖，或者JupyterLab命令参数`--app-dir`覆盖。

通过`jupyter lab path`查看。

```shell
$ jupyter lab path
(HXJupyterhub) [root@master01 ~]# jupyter lab path 
Application directory:   /opt/miniconda/envs/HXJupyterhub/share/jupyter/lab   # 只有安装jupyterlab的用户可写，其他人只读
User Settings directory: /root/.jupyter/lab/user-settings
```

### User Settings Directory

 user-level settings for JupyterLab extensions

- 默认在：$HOME/.jupyter/lab/user-settings/；
- 可以被`JUPYTERLAB_SETTINGS_DIR`环境变量覆盖；

### Workspaces Directory

stores workspaces，contain the state of JupyterLab: the files that are currently open, the layout of the application areas and tabs, etc. 

- 默认在：$HOME/.jupyter/lab/workspaces/；
- 可以被`JUPYTERLAB_WORKSPACES_DIR `环境变量覆盖；

### NoteBook的工作目录

```python
# 单独的Jupyterlab，通过jupyter-lab --genereate-config，生成配置文件后修改
c.NotebookApp.notebook_dir

# 如果是jupyterhub，修改/etc/jupyterhub/jupyterhub_config.py
c.Spawner.notebook_dir = '~'
```



## 安装kernel

### 支持的kernel

sql？



## 扩展

[jupyter-resource-usage](https://github.com/jupyter-server/jupyter-resource-usage) 



## jupterhub

### 配置

*c.Spawner.notebook_dir = Unicode('')*

- jupyterlab左侧的默认目录结构树的根目录



### 多用户方案

#### k8s



