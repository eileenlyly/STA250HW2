//  Copyright (c) 2014 Yu Liu. All rights reserved.
//
#include "ThreadRead.h"

int main(int nargs, char *argv[]){
	char **files = getFiles(FILE_DIR);
	if(!files){
		printf("Check your file dirctory!\n");
		return 0;
	}
    pthread_mutex_t  fileMutex;
	int flag = pthread_mutex_init(&fileMutex,NULL);
	if(flag != 0){
		perror("Mutex initalization failed!\n");  
		return 0;
	}
	pthread_t threads[THREAD_NUM];
	
	FreqTable * tables = constructFT(FILE_NUM, files);
	for(int i = 0; i < THREAD_NUM; i++){
		flag = pthread_create(&threads[i], NULL, assignTables, (void *)tables);
		if(flag != 0) {
			printf("Error when creating thread %d !\n", i+1);  
			return 0;
		}
		printf("Thread %d created!\n", i);
	}
	void * val;
	for(int i = 0; i < THREAD_NUM; i++){
		pthread_join(threads[i], &val);
		if(flag != 0) {
			printf("Error when joining thread %d!\n", i+1);  
			return 0;
		}
	}
	combineFT(tables, FILE_NUM);
	free(files);
	free(tables);
	printf("All done!\n");	
	return 1;
}