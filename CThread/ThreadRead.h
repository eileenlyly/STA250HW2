#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <glob.h>
#include <time.h>

# define DATA_DIR "./airlines/*.*"
# define MAX_DELAY 3000
# define MIN_DELAY -1500
# define MAX_NUM_CHARS 2000
# define COL_NUM1 15
# define COL_NUM2 45
# define THREAD_NUM 9

char **getFiles(char *path);
void randomizeFiles(char **files);

typedef struct{
	int size;
	unsigned long *count;
	char * rawfile;
} FreqTable;

extern int FILE_NUM;
FreqTable *constructFT(int n, char **files);
void *assignTables(void *arg);
int getNext(int cur);
void readDelays(FreqTable *tables, int first, int last);
int getDelayVal(char *line, int type);
void combineFT(FreqTable *tables, int n);
void freeFiles(char **files);
void deconstructFT(FreqTable *tables, int n);
//extern pthread_mutex_t  fileMutex;
