# Shell脚本参数获取

关于参数的内置变量

- **$#** : 脚本或函数的参数个数；
- **$$** : Shell本身的PID
- **$0** : 脚本名称； **$1, $2** … ： 脚本或函数的位置参数；



对于命名的参数获取：**getopt**

getopt命令不是一个标准的unix命令，但它在大多数Linux的发行版中都自带了有，如果没有，也可以从[getopt官网](http://software.frodo.looijaard.name/getopt/)上下载安装。

```shell
ARGS=`getopt -o ab:c:: --long along,blong:,clong:: -n 'example.sh' -- "$@"`
```

- **-o或--options**选项后面接可接受的短选项，如ab:c::，表示可接受的短选项为-a -b -c，其中-a选项不接参数，-b选项后必须接参数，-c选项的参数为可选的；

- **-l**或**--long**选项后面接可接受的长选项，用逗号分开，冒号的意义同短选项。



示例：

```bash
# /bin/bash

show_usage(){
    msg="Generate git two commits diff files, echo modified file with a diff file.\n\
usage: \n\
-b,--begin-commit-id: begin commit id(inclusive) \n\
-e,--end-comiti-d: end commit id(inclusive) \n\
-h,--help"
    echo -e $msg
}

# 解析命令
ARGS=`getopt -o b:e:h -l begin-commit-id:,end-comiti-id:,help -- "$@"`
if [ $? != 0 ]; then
    echo "Error: args is not valid, use -h to show usage help!"
    exit -1
fi

# 将规范化后的命令行参数分配至位置参数（$1,$2,...)
eval set -- "${ARGS}"

begin_commit_id=""
end_commit_id=""

while [ -n "$1" ]
do
        case "$1" in
            -b|--begin-commit-id)
                begin_commit_id=$2
                shift 2
                ;;
            -e|--end-commit-id)
                end_commit_id=$2
                shift 2
                ;;
            -h|--help)
                show_usage
                shift
                ;;
            --)
                break
                ;;
            *)
                echo "unknown args $1"
                exit -1
                ;;
        esac
done

diff_files=$(git diff ${begin_commit_id} ${end_commit_id} --name-only)

if [[ -z $begin_commit_id || -z $end_commit_id ]]; then
    echo $show_usage
    exit -1
fi
```

