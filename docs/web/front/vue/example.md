# 示例

简单示例

```html
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8" />
    <title> Simple Vue Example</title>
    <script src="js/vue.js" type="text/javascript" charset="utf-8"></script>
  </head>
  <body>
    <div id="app-2">
      <span v-bind:title="message">
        鼠标悬停几秒钟查看此处动态绑定的提示信息！
      </span>
    </div>
  </body>
  <script>
    var app2 = new Vue({
      el: '#app-2',
      data: {
        message: '页面加载于 ' + new Date().toLocaleString()
      }
    })
  </script>
</html>
```

