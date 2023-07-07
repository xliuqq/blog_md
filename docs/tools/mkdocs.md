# mkdocs

> 一般使用 [material](https://squidfunk.github.io/mkdocs-material/) 样式。

## 安装

```
pip install mkdocs
pip install mkdocs-material
```

## 配置

### 配置 Github 仓库链接

```yaml
repo_url: https://github.com/xliuqq/blog_md
repo_name: xliuqq/blog_md
theme:
	icon:
    	repo: fontawesome/brands/github
```



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



### PyMdown Extensions

python markdown 插件，支持更多的markdown格式。

```yaml
markdown_extensions:
  - pymdownx.highlight:
      anchor_linenums: true
  - pymdownx.arithmatex:
      generic: true
  - pymdownx.inlinehilite
  - pymdownx.snippets
  - pymdownx.superfences
```



### 数学公式

参考：[MathJax - Material for MkDocs (squidfunk.github.io)](https://squidfunk.github.io/mkdocs-material/reference/mathjax/?h=math#mkdocsyml)



## plugins

集合列表

https://github-wiki-see.page/m/mkdocs/mkdocs/wiki/MkDocs-Plugins#navigation--page-building

### search（内置）

https://www.mkdocs.org/

采用lunr进行中文搜索，当前不支持中文（2021-05-17）。

#### [mkdocs-markdownextradata-plugin](https://github.com/rosscdh/mkdocs-markdownextradata-plugin)

A MkDocs plugin that injects the mkdocs.yml extra variables into the markdown template。

### mkdocs-git-revision-date-localized-plugin

support for adding the date of last update and creation of a document at the bottom of each page

## 博客支持

https://squidfunk.github.io/mkdocs-material/setup/setting-up-a-blog/#posts

