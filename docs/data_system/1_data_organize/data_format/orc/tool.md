# Orc Tool

https://orc.apache.org/docs/java-tools.html

```kotlin
% java -jar orc-tools-X.Y.Z-uber.jar <sub-command> <args>
```

## Meta

- meta - print the metadata of an ORC file

```shell
-j : format the output in JSON
-p : pretty print the output
-t : print the timezone of the writer
–rowindex : print the row indexes for the comma separated list of column ids
–recover : skip over corrupted values in the ORC file
–skip-dump : skip dumping the metadata
–backup-path : when used with –recover specifies the path where the recovered file is written
```

## Data 

- data - print the data of an ORC file

## Scan

- scan (since ORC 1.3) - scan the data for benchmarking

## Convert

- convert (since ORC 1.4) - convert JSON files to ORC

```shell
-o : Sets the output ORC filename, which defaults to output.orc
```

## JSON Schema

- json-schema (since ORC 1.4) - determine the schema of JSON documents