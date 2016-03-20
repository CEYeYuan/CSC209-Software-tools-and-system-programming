#include "mapreduce.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "safe.h"


int main(){
	char *file_name = malloc(sizeof(char)*MAX_FILENAME);
	while(safe_read(STDIN_FILENO,file_name,32)>0){
		fprintf(stderr,"child processing %s\n",file_name);
	}
	return 0;
}