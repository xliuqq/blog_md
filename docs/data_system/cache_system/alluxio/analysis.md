[toc]



## 分析

### **FileInStream中的seek方法**

- 如果seek的位置在同一个Block： 
  - BlockInStream的seek方法；
  - BlockInStream的closeDataReader方法；
    - mCurrentChunk.close() ，这是个NioBuffer的close
      - CLUSTER_CONF：USER_LOCAL_READER_CHUNK_SIZE_BYTES（本地Worker, 8M），决定CHUNK的大小；
      - chunk 是对**文件内容的memory map**，作为ByteBuffer；
      - 调用BufferUtils.cleanDirectBuffer，是个同步的方法。

​				b. mDataReader.close()



### **如果Alluxio的读方式是NO_CACHE，其额外开销？**

