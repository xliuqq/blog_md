[toc]

# API

说明

- `ParquetOutputFormat` ：需要 WriteSupport 实现类， to write your own objects to an event based RecordConsumer.
- `ParquetInputFormat`：需要 ReadSupport 实现类， to materialize your own objects by implementing a RecordMaterializer

## 读取

### 示例

```java
// ParquetFileReader的读取方式
List<SimpleGroup> simpleGroups = new ArrayList<>();
ParquetFileReader reader = ParquetFileReader.open(HadoopInputFile.fromPath(new Path(filePath), new Configuration()));
MessageType schema = reader.getFooter().getFileMetaData().getSchema();
List<Type> fields = schema.getFields();
PageReadStore pages;
while ((pages = reader.readNextRowGroup()) != null) {
	long rows = pages.getRowCount();
	MessageColumnIO columnIO = new ColumnIOFactory().getColumnIO(schema);
	RecordReader recordReader = columnIO.getRecordReader(pages, new GroupRecordConverter(schema));

	for (int i = 0; i < rows; i++) {
		SimpleGroup simpleGroup = (SimpleGroup) recordReader.read();
		simpleGroups.add(simpleGroup);
	}
}
reader.close();

// ParqauetReader 使用 InternalParquetRecordReader（内部使用 ParquetFileReader）
GroupReadSupport readSupport = new GroupReadSupport();
Builder<Group> reader= ParquetReader.builder(readSupport, new Path(inPath));
ParquetReader<Group> build=reader.build();Group line=null;
while((line=build.read())!=null){
    Group time= line.getGroup("time", 0);
    //通过下标和字段名称都可以获取
    /*System.out.println(line.getString(0, 0)+"\t"+time.getInteger(0, 0);*/
    System.out.println(line.getString("city", 0) +"\t"+ time.getInteger("ttl", 0));
}
reader.close()
```



TODO：如何读成 Java 对象



### 映射下推

Parquet中**原生就支持映射下推**，执行查询的时候可以通过Conguration传递需要**读取的列的信息**（Schema），在扫描一个行组时，只扫描对应的列。

```java
Types.MessageTypeBuilder builder = Types.buildMessage();
builder.addField(new PrimitiveType(Type.Repetition.REQUIRED, PrimitiveType.PrimitiveTypeName.BINARY, "city"));
MessageType querySchema = builder.named("Test");

configuration.set(ReadSupport.PARQUET_READ_SCHEMA, querySchema.toString());
```

### 谓词下推

通过将一些过滤条件尽可能的在最底层执行可以减少每一层交互的数据量，从而提升性能；

Parquet在**每个行组中的列都在存储时进行了统计信息的记录**，包括最小值，最大值，空值个数。

```java
FilterCompat.Filter filter = FilterCompat.get(eq(binaryColumn("city"), Binary.fromString("nanjing")));

GroupReadSupport readSupport = new GroupReadSupport();
Builder<Group> reader = ParquetReader.builder(readSupport, new Path(inPath)).withFilter(filter).withConf(configuration);
ParquetReader<Group> build = reader.build();
```



## 写入

### 示例

```java
MessageType schema = MessageTypeParser.parseMessageType("message Pair {\n" +
		" required binary city (UTF8);\n" +
		" required binary ip (UTF8);\n" +
		" repeated group time {\n"+
		　　" required int32 ttl;\n"+
	   　　 " required binary ttl2;\n"+
		"}\n"+
	  "}");
GroupFactory factory = new SimpleGroupFactory(schema);
Path path = new Path(outPath);
Configuration configuration = new Configuration();
GroupWriteSupport writeSupport = new GroupWriteSupport();
writeSupport.setSchema(schema,configuration);
ParquetWriter<Group> writer = new ParquetWriter<Group>(path,configuration,writeSupport);
//把本地文件读取进去，用来生成parquet格式文件
BufferedReader br =new BufferedReader(new FileReader(new File(inPath)));
String line="";
Random r=new Random();
while((line=br.readLine())!=null){
   String[] strs=line.split("\\s+");
   if(strs.length==2) {
	   Group group = factory.newGroup()
			   .append("city",strs[0])
			   .append("ip",strs[1]);
	   Group tmpG =group.addGroup("time");
	   tmpG.append("ttl", r.nextInt(9)+1);
	   tmpG.append("ttl2", r.nextInt(9)+"_a");
	   writer.write(group);
   }
}
writer.close();
```



TODO：将 Java PoJo 对象直接写为Parquet对象。