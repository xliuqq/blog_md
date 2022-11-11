# mkdocs

> 一般使用 [material](https://squidfunk.github.io/mkdocs-material/) 样式。

## 安装

```
pip install mkdocs
pip install mkdocs-material
```

## 配置

### 多语言支持

https://squidfunk.github.io/mkdocs-material/setup/changing-the-language/#site-language-selector

 ```yaml
 extra:
   alternate:
     - name: English
       link: /en/ 
       lang: en
     - name: Deutsch
       link: /zh/
       lang: zh
 ```



### 图片

通过在`docs`建立`img`目录，里面放入图片，在md文件中通过`img/a.png`引入。



### plugins

集合列表

https://github-wiki-see.page/m/mkdocs/mkdocs/wiki/MkDocs-Plugins#navigation--page-building

#### search（内置）

https://www.mkdocs.org/

采用lunr进行中文搜索，当前不支持中文（2021-05-17）。

#### [mkdocs-markdownextradata-plugin](https://github.com/rosscdh/mkdocs-markdownextradata-plugin)

A MkDocs plugin that injects the mkdocs.yml extra variables into the markdown template。



### 博客支持

https://squidfunk.github.io/mkdocs-material/setup/setting-up-a-blog/#posts



### PyMdown Extensions

python markdown 插件，支持更多的markdown格式。

