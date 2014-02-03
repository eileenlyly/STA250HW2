Parallel Computing with Hadoop
=====================

Algorithm
---------
I apply Hadoop to get the frequency table of the delay data. The idea is very similar to the [classic WordCount problem](http://wiki.apache.org/hadoop/WordCount). The key of Mapper is the delay column, value is 1. Reducer is the same as WordCount.
Then I wrote a simple R code to calculate the mean, sd and median.
Set DFS Block Size
---------
The number of maps is driven by the number of DFS blocks in the input files. Here I adjust DFS block size to adjust the number of maps. The default block size for Hadoop is 128 MB. By modifying `hdfs-site.xml`, we can set it to any size we want (in byte). Note that this value must be a multiplier of 512 (the default size of io.bytes.per.checksum), or hadoop will throw the exception `"block size should be a multiple of io.bytes.per.checksum"`.
For example, we can set DFS block size to be 32MB by adding the following lines between the `configuration` tag.
`<property>
    <name>dfs.block.size</name>
    <value>33554432</value>
</property>
`
An alternative way to set the number of map tasks is by manually set the `JobConf` in the Java code:
`conf.setNumMapTasks(int num);`
But this will not set the number below that which Hadoop determines via splitting the input data.
Run Hadoop
---------
I assume you have already setup Hadoop properly. You can find a bunch of tutorials on how to install Hadoop.
>- Start Hadoop
`sbin/start-all.sh`
Use `jps` to check all the nodes are ready.
- Copy data(.csv) from local to DFS
`bin/hadoop dfs -put <pathtodataonlocal> <pathtoDFS>`
- Compile Java
`javac -cp <hadoopcoreclass> getFT.java`
`jar -cf getFT.jar`
- Run `jar`
`bin/hadoop jar getFT.jar getFT <input> <output>` 
- Save result to local
`bin/hadoop dfs get <output>/part-00000 <pathtolocal>`
>

Execution Time
---------
We can get the details of Hadoop jobs from the output in terminal or the log files.

| DFSBlockSize     |  Time | 
| :--------: | :------:| 
| 128 MB  | 4644 s | 
| 32 MB     |  1558 s| 
| 8 MB      |  929 s| 
| 4 MB     |   960 s | 
| 2 MB      |  1147 s | 

The shortest execution time comes when DFS block size is between 4 MB and 8 MB.
