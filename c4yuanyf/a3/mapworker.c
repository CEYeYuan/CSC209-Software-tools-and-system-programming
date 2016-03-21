#include "mapreduce.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "safe.h"

int main(){
	char *file_name = malloc(sizeof(char)*MAX_FILENAME);
	while(safe_read(STDIN_FILENO,file_name,MAX_FILENAME) > 0){
		//fprintf(stderr,"child processing %s\n",file_name);
		FILE *file = safe_fopen(file_name,"r");
		char *chunk = malloc(sizeof(char)*READSIZE);
		while(fread(chunk,READSIZE-1,1,file) > 0){
			chunk[READSIZE-1] = '\0';
			map(chunk,STDOUT_FILENO);
		}
		safe_fclose(file);
	}
	return 0;
}