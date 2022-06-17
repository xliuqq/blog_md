

cmake 在 `find_path` 和 `find_library` 时，会搜索一些默认的路径。当我们将一些lib安装在非默认搜索路径时，cmake就没法搜索到了。这是我们需要添加路径。方法如下：

set(CMAKE_INCLUDE_PATH "include_path")

set(CMAKE_LIBRARY_PATH "lib_path")



find_library (<VAR> name1 [path1 path2 ...])