#include "ThreadRead.h"

char **getFiles(char *s){
	char **files = NULL;
	glob_t data;
	glob(s, 0, NULL, &data);
    if(data.gl_pathc == 0) return NULL;
    FILE_NUM = (int)data.gl_pathc;
    printf("%d files found:\n",FILE_NUM);
    files = (char **)malloc(sizeof(char*) * FILE_NUM);
	for(int i = 0; i < data.gl_pathc; i++){
		files[i] = strdup(data.gl_pathv[i]);
        printf("%s\n", files[i]);
	}
	return files;
}

void randomizeFiles(char **files){
	for(int i = 0; i < FILE_NUM; i++){
		srand((int)time(0));
		int ran = rand() % FILE_NUM;
		if(ran != i){
			char tmp[30];
			strcpy(tmp, files[i]);
			strcpy(files[i], files[ran]);
			strcpy(files[ran], tmp);
            //printf("%s swap with %s\n", files[i], files[ran]);
		}
	}
    return;
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

int getNext(int cur){
	int m = FILE_NUM % THREAD_NUM;
	int k = (int) FILE_NUM / THREAD_NUM;
	if(cur >= m*k+m) return cur+k-1;
	else return cur+k;
}

void readDelays(FreqTable * tables, int first, int last){
	for(int i = first; i <= last; i++){
		FILE *fp;
		fp = fopen(tables[i].rawfile, "r");
		if(!fp){
            printf("Fail to find file!\n");
            return;
        }
		int type = 1;
        for(int j = 0; j < strlen(tables[i].rawfile); j++){
            if(tables[i].rawfile[j] == '_') type = 2;
        }
        //printf("%s has type %d\n", tables[i].rawfile, type);
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
	int field = COL_NUM1;
	if(type == 2) field = COL_NUM2;
	char *val = NULL;
	for(int i = 0, col = 1; i < MAX_NUM_CHARS; i++){
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
		for(int j = 0; j < res -> size; j++)
			res -> count[j] += (tables+i) -> count[j];
	}
    /*for(int i = 0; i < MAX_DELAY - MIN_DELAY; i++){
		if(res -> count[i] != 0){
			printf("%d %lu\n", i + MIN_DELAY, res -> count[i]);
		}
	}*/
	FILE *fp;
	fp = fopen("./output.txt","w");
	for(int i = 0; i < res -> size; i++){
		if(res -> count[i] != 0){
			fprintf(fp, "%d %lu\n", i + MIN_DELAY, res -> count[i]);
		}
	}
	fclose(fp);
    deconstructFT(res, 1);
    printf("Result written to output.txt!\n");
	return;
}

void freeFiles(char **files){
    for(int i = FILE_NUM - 1; i <= 0; i++)
        free(files[i]);
}

void deconstructFT(FreqTable *tables, int n){
    for(int i = n-1; i <= 0; i++){
        free((tables+i) -> count);
        free(tables+i);
    }
    return;
}