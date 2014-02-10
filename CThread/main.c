//  Copyright (c) 2014 Yu Liu. All rights reserved.
//
#include "ThreadRead.h"

pthread_mutex_t fileMutex;
int FILE_NUM = 0;

int main(int nargs, char *argv[]){
    //timer starts now.
    time_t start, end;
    time(&start);
    //get the array of file paths
	char **files = getFiles(DATA_DIR);
	if(!files){
		printf("Check your file dirctory!\n");
		return 0;
	}
    //randomize file array
    randomizeFiles(files);
    //initial mutex lock, it is a static variable
	int flag = pthread_mutex_init(&fileMutex,NULL);
	if(flag != 0){
		perror("Mutex initalization failed!\n");
		return 0;
	}
	pthread_t threads[THREAD_NUM];
	//constuct freqTable array, initialize frequency table array and the file path
	FreqTable * tables = constructFT(FILE_NUM, files);
    //create threads in loop
	for(int i = 0; i < THREAD_NUM; i++){
		flag = pthread_create(&threads[i], NULL, assignTables, (void *)tables);
		if(flag != 0) {
			printf("Error when creating thread %d !\n", i+1);
			return 0;
		}
		printf("Thread %d created!\n", i);
	}
	void * val;
    //join threads in loop
	for(int i = 0; i < THREAD_NUM; i++){
		pthread_join(threads[i], &val);
		if(flag != 0) {
			printf("Error when joining thread %d!\n", i+1);
			return 0;
		}
	}
    //combine all frequency tables and write to ouput file
	combineFT(tables, FILE_NUM);
    //free space to avoid the leak of memory
	freeFiles(files);
	deconstructFT(tables, FILE_NUM);
    //timer ends now
    time(&end);
	printf("All done in %.2f seconds.\n", difftime(end,start));
	return 1;
}
// give each thread a block of tables
void *assignTables(void *arg){
	static int cur = 0;
    //all tables are assigned
    if(cur == FILE_NUM) {
        printf("No file to read!\n");
        return (void *) NULL;
    }
	FreqTable *tables = (FreqTable *)arg;
    //lock
	pthread_mutex_lock(&fileMutex);
	int first = cur;
	int last = getNext(cur);
	cur = last + 1;
	pthread_mutex_unlock(&fileMutex);
    //unlock
    printf("Read file %d to %d\n", first+1, last+1);
    //read the tables
	readDelays(tables, first, last);
	pthread_exit(NULL);
}