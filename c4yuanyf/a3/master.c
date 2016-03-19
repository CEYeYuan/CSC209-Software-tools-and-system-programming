#include <stdio.h>
#include <stdlib.h>	
#include "mapreduce.h"
#include <unistd.h>
#include <getopt.h>

int create_map_workers(int num);
int create_reduce_workers(int num);
void safe_close(int num);
int main(int argc,char** argv){
/*
	mapreduce -m 5 -r 3 -d texts.
*/
	int map_worker = 2;
	int reduce_worker = 2;
	char *dir = NULL;//not set yet
	int option;
	while((option = getopt(argc,argv,"m:r:d:")) != -1){
		switch(option){
			case 'm':
				map_worker = atoi(optarg);
				break;
			case 'r':
				reduce_worker = atoi(optarg);
				break;
			case 'd':
				dir = optarg;
				break;
			default:
				fprintf(stderr, "Correct usage: mapreduce -m 2 -r 2 -d dir\n");
				return -1;
		}
	}
	if(dir == NULL){//must be inited
		fprintf(stderr, "Correct usage: mapreduce -m 2 -r 2 -d dir\n");
		return -1;
	}

	int fd[1+map_worker+reduce_worker][2];
	if (pipe(fd[0]) == -1){
		fprintf(stderr, "Error creating pipe");
	}
	int ret = fork();
	if(ret == -1){
		//child process, close reading pipe
		safe_close(fd[0][0]);
	}else{
		safe_close(fd[0][1]);
	}


	// if (create_map_workers(map_worker) != map_worker){
	// 	//should never happen
	// 	fprintf(stderr, "Error in creating map worker\n");
	// 	return -1;
	// }
	// if (create_reduce_workers(reduce_worker) != reduce_worker){
	// 	//should never happen
	// 	fprintf(stderr, "Error in creating reduce worker\n");
	// 	return -1;
	// }
	
}

void safe_close(int fd){
	int ret = close(fd);
	if(ret == -1){
		printf("Error closing pipe\n");
		exit(-1);
	}
}