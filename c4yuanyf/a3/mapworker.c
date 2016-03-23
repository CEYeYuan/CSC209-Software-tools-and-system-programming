#include "mapreduce.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "safe.h"

int main(){
	char *file_name = safe_malloc(sizeof(char) * MAX_FILENAME);
	char *chunk = safe_malloc(sizeof(char)*READSIZE);
	//read file name from the master process and call map function
	//on that file, return the result to the master process
	while(safe_read(STDIN_FILENO, file_name, MAX_FILENAME) > 0){
		FILE *file = safe_fopen(file_name, "r");	
		while(fread(chunk, READSIZE-1, 1, file) > 0){
			//make sure the chunk is null-terminated
			chunk[READSIZE-1] = '\0';
			map(chunk, STDOUT_FILENO);
		}
		safe_fclose(file);
	}
	free(file_name);
	free(chunk);
	return 0;
}