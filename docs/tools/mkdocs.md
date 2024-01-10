---
hide:
  - tags
tags:
  - mkdocs
---



# mkdocs

> 一般使用 [material](https://squidfunk.github.io/mkdocs-material/) 样式。

## 安装

```
pip install mkdocs-material -i https://pypi.tuna.tsinghua.edu.cn/simple
pip install mkdocs-material -i https://pypi.tuna.tsinghua.edu.cn/simple
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

通过 `<img>`标签引入时，需要进行如下设置：

- 相关问题和解释可见：[markdown - configuring image filepaths correctly in mkdocs - Stack Overflow](https://stackoverflow.com/questions/71074662/configuring-image-filepaths-correctly-in-mkdocs)

```yaml
use_directory_urls: false
```



## PyMdown Extensions

python markdown 插件，支持更多的markdown格式。

```yaml
markdown_extensions:
  - pymdownx.highlight:
      anchor_linenums: true
  - pymdownx.inlinehilite
  - pymdownx.snippets
  - pymdownx.superfences
```

snippets:

- adds the ability to embed content from arbitrary files into a document,

### [数学公式MathJax](https://squidfunk.github.io/mkdocs-material/reference/mathjax/?h=math#mkdocsyml)

`mkdocs.yaml`

```yaml
markdown_extensions:
  - pymdownx.arithmatex:
      generic: true

extra_javascript:
  - javascripts/mathjax.js
  - https://polyfill.io/v3/polyfill.min.js?features=es6
  - https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-mml-chtml.js
```

`javascripts/mathjax.js`

```javascript
window.MathJax = {
  tex: {
    inlineMath: [["\\(", "\\)"]],
    displayMath: [["\\[", "\\]"]],
    processEscapes: true,
    processEnvironments: true
  },
  options: {
    ignoreHtmlClass: ".*|",
    processHtmlClass: "arithmatex"
  }
};

document$.subscribe(() => { 
  MathJax.typesetPromise()
})
```

### [Diagrams](https://squidfunk.github.io/mkdocs-material/reference/diagrams/?h=mermaid)

支持 mermaid

```yaml
markdown_extensions:
  - pymdownx.superfences:
      custom_fences:
        - name: mermaid
          class: mermaid
          format: !!python/name:pymdownx.superfences.fence_code_format
```



### [Tabbed](https://squidfunk.github.io/mkdocs-material/reference/content-tabs/)

配置

```yaml
markdown_extensions:
  - pymdownx.superfences
  - pymdownx.tabbed:
      alternate_style: true 
```



=== "C"

    ``` c
    #include <stdio.h>
    
    int main(void) {
      printf("Hello world!\n");
      return 0;
    }
    ```

=== "C++"

    ``` c++
    #include <iostream>
    
    int main(void) {
      std::cout << "Hello world!" << std::endl;
      return 0;
    }
    ```



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

