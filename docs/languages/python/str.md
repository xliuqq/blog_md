# 字符串

## format

```python
a = "I'm %s. I'm %d year old" % ('Vamei', 99)
# or
"I'm %(name)s. I'm %(age)d year old" % {'name':'Vamei', 'age':99}
# or
"I'm {name}. I'm {age} year old".format(name='Vamei', age=99)
# or
name='Vamei', age=99
f"I'm {name}. I'm {age} year old"
```



| 格式                          |                  |                      |                                    |
| ----------------------------- | ---------------- | -------------------- | ---------------------------------- |
| %s  字符串 (采用str()的显示)  | %d  十进制整数   | %e  指数 (基底写为e) | %g  指数(e)或浮点数 (根据显示长度) |
| %r  字符串 (采用repr()的显示) | %i  十进制整数   | %E  指数 (基底写为E) | %G  指数(E)或浮点数 (根据显示长度) |
| %c  单个字符                  | %o  八进制整数   | %f  浮点数           | %%  字符"%"                        |
| %b  二进制整数                | %x  十六进制整数 | %F  浮点数，与上相同 |                                    |

 

## string 开头r b u f

```python
# bytes字节符，打印以b开头
b'input\n'
# 非转义原生字符，经处理'\n'变成了'\\'和'n'。也就是\n表示的是两个字符，而不是换行。
r'input\n'               
# unicode编码字符，python3默认字符串编码方式。
u'input\n'          
# 变量替换
f"I'm {name}. I'm {age} year old" 
```

如果要将现有字符串转换为原始字符串，那么我们可以像下面那样重新分配 

```python
s1 = "welcome\tto\tPython" 
raw_s1 = "%r" % s1 
print(raw_s1) # 会打印 welcome\tto\tPython
```



