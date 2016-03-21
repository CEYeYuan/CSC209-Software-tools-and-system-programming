#include "mapreduce.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "safe.h"

int main(){
	Pair pair;
	while(safe_read(STDIN_FILENO,&pair,sizeof(pair)) > 0){
		//fprintf(stderr,"child processing %s\n",pair.key);
	}

	FILE *fopen = safe_fopen("%d.out",getpid()){
		
	}
	return 0;
}