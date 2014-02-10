


Parallel Computing with Multiple Threads in C
=====================

Data Structure
---------
I define a structure `FreqTable` to store the information for each csv file. Array `count` stores the count numbers between `MIN_DELAY` and `MAX_DALAY`. `rawfile` stores the csv file it points to. So each `FreqTable` stands for a file and its frequency table.

Algorithm
---------
I use a loop to generate threads. Then the thread is designed to execute `assignTables`. This function simulates a thread pool, which assigns each thread a block of `FreqTable` to read by `readDelays`. For example, if we have 10 files to read by 4 threads, then the first two threads should read 3 files and the other two read 2 files. When calculating which block of tables each thread should read, I use a `Mutex lock` to avoid the conflicts.


The total time is determined by the longest execution time of all  threads. Since the raw files are not uniformly distributed (the files between 1988 - 2007.csv are significantly larger than the others), some threads will have longer execution time if we simply assign them tables sequentially. The solution I use is to randomize the sequence of the file array `char **file`. Statistically, each block of the same size should have equal amount of data.
At last, we use `combineFT` to sum up all the `count` in every `Freqtable` and output the result.

Run code
---------

You should modify the data directory in `ThreadRead.h`. 
```
# define DATA_DIR "./airlines/*.*"
```
The directory should contain the data only.
Run the code in terminal or IDE (I tested in Xcode5). The result will be written to `output.txt` in your working directory.
Then calculate the mean, sd and median with the simple R code provided.

Result
---------
Result is the same as we usually got:

**Mean: 6.5665**
**SD = 31.5563** 
**Median = 0**

Execution Time
---------
On my computer, the optimal execution time comes when Thread Number is set to be 9.

| Thread Number     |  Time | 
| :--------: | :------:| 
| 8  | 796 s | 
| 9     |  518 s| 
| 10      |  587 s| 

The running time in R is trivial, which is only 0.3s.
