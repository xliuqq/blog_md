# 类

## super

https://mozillazg.com/2016/12/python-super-is-not-as-simple-as-you-thought.html

- Python 3 可以使用直接使用 **super().xxx 代替 super(Class, self).xxx**

- super 包含了两个非常重要的信息: 一个 **MRO(Method Resolution Order) 列表**以及 MRO 中的一个类。当以如下方式调用 super 时: super(a_type, obj)  MRO 列表指的是 type(obj) 的 MRO 列表, MRO 中的那个类就是 a_type , 同时 isinstance(obj, a_type) == True 。

- 举个栗子, 有个 MRO 列表:

  [A, B, C, D, E, object]

  下面的调用:

  super(C, A).foo()

  super 只会从 C 之后查找，即: 只会在 D 或 E 或 object 中查找 foo 方法。