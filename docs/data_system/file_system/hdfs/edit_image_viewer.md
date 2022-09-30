[toc]

# Offline Edit Viewer

**解析Edit log文件，离线工具**，不需要Hadoop集群运行。binary和xml可以互相转换。

输入格式支持：1）binary；2）xml

输出格式支持：1）binary；2）xml；3）stats（统计信息，不可以转回Edits文件）

## XML Processor

默认的处理器

```shell
bash$ bin/hdfs oev -p xml -i edits -o edits.xml
```

结果的格式如下：

```xml
<?xml version="1.0" encoding="UTF-8"?>
   <EDITS>
     <EDITS_VERSION>-64</EDITS_VERSION>
     <RECORD>
       <OPCODE>OP_START_LOG_SEGMENT</OPCODE>
       <DATA>
         <TXID>1</TXID>
       </DATA>
     </RECORD>
     <RECORD>
       <OPCODE>OP_UPDATE_MASTER_KEY</OPCODE>
       <DATA>
         <TXID>2</TXID>
         <DELEGATION_KEY>
           <KEY_ID>1</KEY_ID>
           <EXPIRY_DATE>1487921580728</EXPIRY_DATE>
           <KEY>2e127ca41c7de215</KEY>
         </DELEGATION_KEY>
       </DATA>
     </RECORD>
     <RECORD>
   ...remaining output omitted...
```

## Binary Processor

```shell
bash$ bin/hdfs oev -p binary -i edits.xml -o edits
```

## Stats Processor

```shell
bash$ bin/hdfs oev -p stats -i edits -o edits.stats
```

结果格式如下：VERSION就是HDFS NameNode的VERSION

```shell
   VERSION                             : -64
   OP_ADD                         (  0): 8
   OP_RENAME_OLD                  (  1): 1
   OP_DELETE                      (  2): 1
   OP_MKDIR                       (  3): 1
   OP_SET_REPLICATION             (  4): 1
   OP_DATANODE_ADD                (  5): 0
   OP_DATANODE_REMOVE             (  6): 0
   OP_SET_PERMISSIONS             (  7): 1
   OP_SET_OWNER                   (  8): 1
   OP_CLOSE                       (  9): 9
   OP_SET_GENSTAMP_V1             ( 10): 0
   ...some output omitted...
   OP_APPEND                      ( 47): 1
   OP_SET_QUOTA_BY_STORAGETYPE    ( 48): 1
   OP_INVALID                     ( -1): 0
```

## 命令参数

| Flag                                  | Description                                                  |
| :------------------------------------ | :----------------------------------------------------------- |
| [`-i` ; `--inputFile`] *input file*   | 必须，输入文件                                               |
| [`-o` ; `--outputFile`] *output file* | 必须，输出文件，覆盖模式                                     |
| [`-p` ; `--processor`] *processor*    | 处理器类型，binary/xml/stats                                 |
| [`-v` ; `--verbose`]                  | 结果输出一份到控制台                                         |
| [`-f` ; `--fix-txids`]                | 重新对输入中的事务ID编号，以便没有间隙或无效的事务ID。       |
| [`-r` ; `--recover`]                  | 使用恢复模式读取二进制编辑日志，可以跳过编辑日志中损坏的部分 |
| [`-h` ; `--help`]                     | help信息                                                     |

## 案例学习

如果hadoop群集有问题，并且编辑文件已损坏，则可以**至少保存部分正确的编辑文件**。这可以通过**将二进制编辑转换为XML，手动编辑，然后将其转换回二进制**来完成。

最常见的问题是**编辑文件缺少结束记录（操作码为-1的记录）**。这应该由工具识别，并且XML格式应该正确关闭。

如果XML文件中没有结束记录，则可以在最后一个正确记录之后添加一个结束记录。操作码为-1的记录之后的任何内容都将被忽略。

```xml
  <RECORD>
    <OPCODE>-1</OPCODE>
    <DATA>
    </DATA>
  </RECORD>
```

# Offline Image Viewer

将hdfs fsimage文件的内容转储为人类可读格式，并提供只读WebHDFS API，以便允许**脱机分析和检查Hadoop集群的命名空间**。

- Web是默认的输出处理器。它启动一个HTTP服务器，公开只读WebHDFS API。用户可以使用HTTP REST API交互地调查命名空间。
- XML创建fsimage的XML文档，并包含fsimage中的所有信息。这个处理器的输出可以通过XML工具进行自动处理和分析。由于XML语法的冗长性，这个处理器也将生成最大数量的输出。
- FileDistribution是分析命名空间映像中文件大小的工具
- Delimited (experimental)：生成一个文本文件，其中包含正在构造的索引节点和索引节点共有的所有元素，用分隔符分隔。
- ReverseXML (experimental)：从XML文件重建fsimage。此处理器使创建用于测试的fsimages和在出现损坏时手动编辑fsimages变得容易。

## Web Processor

Web处理器启动一个HTTP服务器，该**服务器公开只读WebHDFSAPI**。用户可以指定addr选项监听的地址（默认为本地主机：5978)

```shell
bash$ bin/hdfs oiv -i fsimage
```

通过hdfs命令可以查看数据

```shell
bash$ bin/hdfs dfs -ls webhdfs://127.0.0.1:5978/
Found 2 items
drwxrwx--* - root supergroup          0 2014-03-26 20:16 webhdfs://127.0.0.1:5978/tmp
drwxr-xr-x   - root supergroup          0 2014-03-31 14:08 webhdfs://127.0.0.1:5978/user
```

可以通过 HTTP REST API 获取 JSON格式的 FileStatus

```shell
bash$ curl -i http://127.0.0.1:5978/webhdfs/v1/?op=liststatus
```

当前支持以下操作：

- [LISTSTATUS](https://hadoop.apache.org/docs/r2.10.0/hadoop-project-dist/hadoop-hdfs/WebHDFS.html#List_a_Directory)
- [GETFILESTATUS](https://hadoop.apache.org/docs/r2.10.0/hadoop-project-dist/hadoop-hdfs/WebHDFS.html#Status_of_a_FileDirectory)
- [GETACLSTATUS](https://hadoop.apache.org/docs/r2.10.0/hadoop-project-dist/hadoop-hdfs/WebHDFS.html#Get_ACL_Status)
- [GETXATTRS](https://hadoop.apache.org/docs/r2.10.0/hadoop-project-dist/hadoop-hdfs/WebHDFS.html#Get_an_XAttr)
- [LISTXATTRS](https://hadoop.apache.org/docs/r2.10.0/hadoop-project-dist/hadoop-hdfs/WebHDFS.html#List_all_XAttrs)
- [CONTENTSUMMARY](https://hadoop.apache.org/docs/r2.10.0/hadoop-project-dist/hadoop-hdfs/WebHDFS.html#Get_Content_Summary_of_a_Directory)

## XML Processor

```shell
bash$ bin/hdfs oiv -p XML -i fsimage -o fsimage.xml
```

结果输出格式如下：

```xml
<?xml version="1.0"?>
   <fsimage>
   <NameSection>
     <genstampV1>1000</genstampV1>
     <genstampV2>1002</genstampV2>
     <genstampV1Limit>0</genstampV1Limit>
     <lastAllocatedBlockId>1073741826</lastAllocatedBlockId>
     <txid>37</txid>
   </NameSection>
   <INodeSection>
     <lastInodeId>16400</lastInodeId>
     <inode>
       <id>16385</id>
       <type>DIRECTORY</type>
       <name></name>
       <mtime>1392772497282</mtime>
       <permission>theuser:supergroup:rwxr-xr-x</permission>
       <nsquota>9223372036854775807</nsquota>
       <dsquota>-1</dsquota>
     </inode>
   ...remaining output omitted...
```

## FileDistribution

```shell
bash$ bin/hdfs oiv -p FileDistribution -maxSize maxSize -step size -i fsimage -o output
```

输出形式如下（可以指定 -format 更可读）：

```shell
Size	NumFiles
4	1
12	1
16	1
20	1
totalFiles = 4
totalDirectories = 2
totalBlocks = 4
totalSpace = 48
maxFileSize = 21
```

## ReverseXML Processor

```shell
bash$ bin/hdfs oiv -p ReverseXML -i fsimage.xml -o fsimage
```

## Delimited Processor

```shell
bash$ bin/hdfs oiv -p Delimited -delimiter delimiterString -i fsimage -o output
```

结果输出格式：

```shell
 Path	Replication	ModificationTime	AccessTime	PreferredBlockSize	BlocksCount	FileSize	NSQUOTA	DSQUOTA	Permission	UserName	GroupName
   /	0	2017-02-13 10:39	1970-01-01 08:00	0	0	0	9223372036854775807	-1	drwxr-xr-x	root	supergroup
   /dir0	0	2017-02-13 10:39	1970-01-01 08:00	0	0	0	-1	-1	drwxr-xr-x	root	supergroup
   /dir0/file0	1	2017-02-13 10:39	2017-02-13 10:39	134217728	1	1	0	0	-rw-r--r--	root	supergroup
   /dir0/file1	1	2017-02-13 10:39	2017-02-13 10:39	134217728	1	1	0	0	-rw-r--r--	root	supergroup
   /dir0/file2	1	2017-02-13 10:39	2017-02-13 10:39	134217728	1	1	0	0	-rw-r--r--	root	supergroup
```

## 命令参数

| **Flag**                          | **Description**                                              |
| :-------------------------------- | :----------------------------------------------------------- |
| `-i`|`--inputFile` *input file*   | 必须，输入文件                                               |
| `-o`|`--outputFile` *output file* | 输出文件名，如果输出是xml，则也会创建个md5校验文件           |
| `-p`|`--processor` *processor*    | 处理器，支持 `Web` (default), `XML`, `Delimited`, `FileDistribution` 和`ReverseXML`. |
| `-addr` *address*                 | Web处理器时指定监听地址(host:port) ，默认 localhost:5978     |
| `-maxSize` *size*                 | FileDistribution处理器，指定文件大小的范围 [0, maxSize]，默认128GB |
| `-step` *size*                    | FileDistribution处理器，，以字节为单位指定分布的粒度 ，默认 2MB |
| `-format`                         | FileDistribution处理器，输出结果格式化为可读形式，默认false  |
| `-delimiter` *arg*                | Delimited处理器的分隔符，默认\t                              |
| `-t`|`--temp` *temporary dir*     | Delimited处理器，使用temporary dir缓存中间结果以生成分隔的输出。如果未设置，则分隔处理器在输出文本之前在内存中构造命名空间。 |
| `-h`|`--help`                     | help信息                                                     |