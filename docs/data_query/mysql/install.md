Windows安装

- 在mysql-8-winx64的文件夹下创建一个名为data的空文件夹；

- 创建一个my.ini的文件，放在bin目录里面。内容为：

  ```ini
  [mysql]
  # 设置mysql客户端默认字符集
  default-character-set=utf8 
  [mysqld]
  #设置3306端口
  port = 3306 
  # 设置mysql的安装目录
  basedir=E:/mysql-8-winx64
  # 设置mysql数据库的数据的存放目录
  datadir=E:/mysql-8-winx64/data
  # 允许最大连接数
  max_connections=200
  # 服务端使用的字符集默认为8比特编码的latin1字符集
  character-set-server=utf8
  # 创建新表时将使用的默认存储引擎
  default-storage-engine=INNODB
  ```

- mysqld --initialize -console **会在控制台打印临时密码**
- mysqld -install
- 启动： net start mysql
- 修改root密码
  - mysql -u root -p
  - 
  - ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'xliu1992'; 

- 停止mysql服务： net stop mysql





时区：



```mysql
// 方法一：使用命令（优点：不需要重启MySQL服务，缺点：一旦MySQL服务被重启，设置就会消失）
mysql> set time_zone = '+8:00';
mysql> set global time_zone = '+8:00';
// 方法二：修改my.ini配置文件（优点：永久保存设置，缺点：需重启MySQL服务）
[mysqld]
// 设置默认时区
default-time_zone='+8:00'
```





