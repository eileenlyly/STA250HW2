#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <glob.h>
#include <time.h>

# define FILE_DIR "airlines/"
# define MAX_DELAY 3000
# define MIN_DELAY -1500
# define MAX_NUM_CHARS 2000
# define COL_NUM1 14
# define COL_NUM2 44
# define FILE_NUM 82
# define THREAD_NUM 10

char **getFiles(char *path);
char **randomizeFiles(char **files);

typedef struct{
	int size;
	unsigned long *count;
	char * rawfile;
} FreqTable;

FreqTable *constructFT(int n, char **files);
void *assignTables(void *arg);
int getNext(int cur);
void readDelays(FreqTable *tables, int first, int last);
int getDelayVal(char *line, int type);
void combineFT(FreqTable *tables, int n);
