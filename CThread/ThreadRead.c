#include "ThreadRead.h"

char **getFiles(char * s){
	char **files = NULL;
	files = (char **)malloc(sizeof(char*) * FILE_NUM);
	glob_t data;
	glob("./*.*", 0, NULL, &data);
	for(int i=0; i<data.gl_pathc; i++){
		files[i] = strdup(data.gl_pathv[i]);
	}
	return files;
}

char **randomizeFiles(char **files){
	for(int i = 0; i < FILE_NUM; i++){
		srand((int)time(0));
		int ran = rand() % FILE_NUM;
		if(ran != i){
			char tmp[20];
			strcpy(tmp, files[i]);
			strcpy(files[i], files[ran]);
			strcpy(files[ran], tmp);
		}		
	}	
	return files;
}

FreqTable *constructFT(int n, char **files){
	FreqTable *t = (FreqTable *) malloc(sizeof(FreqTable) * n);
	for(int i = 0; i < n; i++){
		(t+i) -> size = MAX_DELAY - MIN_DELAY + 1;
		(t+i) -> count = (unsigned long *)malloc(sizeof(unsigned long) * (t+i) -> size);
		if(files) (t+i) -> rawfile = files[i];
		memset((t+i) -> count, 0, sizeof(unsigned long) * (t+i) -> size);
	}	
	return t;
}

void *assignTables(void * arg){
	static int cur = 0;
	tables = (FreqTable *)arg;
	pthread_mutex_lock(&fileMutex);
	int first = cur;
	int last = getNext(cur);
	cur = last + 1;
	pthread_mutex_unlock(&fileMutex);
	if(last == first) return (void *) NULL;
	readDelays(tables, first, last);
	pthread_exit(NULL);
}

int getNext(int cur){
	int m = FILE_NUM % THREAD_NUM;
	int k = (int) FILE_NUM / THREAD_NUM;
	if(cur >= m*k+m-1) return cur+k-1;
	else return cur+k;
}
void readDelays(FreqTable * tables, int first, int last){
	for(int i = first; i <= last; i++){
		FILE *fp;
		fp = fopen(tables[i].rawfile, "r");
		if(!fp) return;
		int type = 1;
		if(tables[i].rawfile[4] == '_') type = 2;
		char line[MAX_NUM_CHARS];
		fgets(line, MAX_NUM_CHARS, fp);
		while(fgets(line, MAX_NUM_CHARS, fp)){
			int val = getDelayVal(line, type);
			if(val == MAX_DELAY) continue;
			tables[i].count[val - MIN_DELAY]++;
		}
		fclose(fp);
	}
	return;
}
int getDelayVal(char *line, int type){
	int field = 15;
	if(type == 2) field = 45;
	char * val = NULL;
	for(int i = field*2, col = 1; i < MAX_NUM_CHARS; i++){
		if(line[i] == ',') col++;
		if(col == field){
			int tmp = ++i;
			val = line + i;
			if(line[i] == '-') i++;
			while(line[i] - '0' >= 0 && '9' - line[i] >= 0) i++;
			if(tmp == i) return MAX_DELAY;
			line[i] = '\0';
			break;
		}
	}
	return atoi(val);
}
void combineFT(FreqTable *tables, int n){
	FreqTable *res = constructFT(1, NULL);
	for(int i = 0; i < n; i++){
		for(int j = MIN_DELAY; j < MAX_DELAY; j++)
			res -> count[j] += (tables+i) -> count[j];
	}
	FILE *fp;
	fp = fopen("output.txt","w");
	for(int i = 0; i < MAX_DELAY - MIN_DELAY; i++){
		if(res -> count[i] != 0){
			fprintf(fp, "%d %lu", i + MIN_DELAY, res -> count[i]);
		}
	}
	fclose(fp);
	return;
	
}