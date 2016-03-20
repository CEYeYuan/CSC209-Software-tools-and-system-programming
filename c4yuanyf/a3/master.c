#include <stdio.h>
#include <stdlib.h>	
#include "mapreduce.h"
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "safe.h"

int create_map_workers(int fd[][2], int num);
int create_reduce_workers(int fd[][2], int num);



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

	int fd[1][2];//fd for ls process
	int map_fd[map_worker*2][2];//fd for the map worker
	int reduce_fd[reduce_worker*2][2];

	if (create_map_workers(map_fd,map_worker) != map_worker){
		//should never happen
		fprintf(stderr, "Error in creating map worker\n");
		return -1;
	}

	safe_pipe(fd[0]);
	int ret = safe_fork();
	if(ret == 0){
		//child process, close reading pipe
		safe_close(fd[0][0]);
		safe_dup2(fd[0][1],STDOUT_FILENO);
		safe_close(fd[0][1]);
		execl("/bin/ls","ls",dir,NULL);
		perror("exec");
	}else{
		//parent process, read from the pipe
		safe_close(fd[0][1]);
		safe_dup2(fd[0][0], STDIN_FILENO);
		safe_close(fd[0][0]);
		int count = 0;
		char *file_name = malloc(sizeof(char)*MAX_FILENAME);
		while(scanf("%s",file_name) > 0){
			int index = count % map_worker;
			fprintf(stdout, "%s to worker %d \n", file_name, index);
			safe_write(map_fd[index*2][1],file_name, 30);
			//safe_write(map_fd[index*2][1], "dumb", 5);
			count++;
		}
	}



	// if (create_reduce_workers(reduce_worker) != reduce_worker){
	// 	//should never happen
	// 	fprintf(stderr, "Error in creating reduce worker\n");
	// 	return -1;
	// }
	printf("%d %d\n",reduce_worker,reduce_fd[0][1]);
	return 0;
	
}

int create_map_workers(int fd[][2],int num){
	int i;
	for(i = 0;i < num;i++){
		//the first pipe is used for sending data to map worker
		//the second pipe is used for receiving data from map worker
		safe_pipe(fd[2*i]);
		safe_pipe(fd[2*i+1]);
		int ret = safe_fork();
		if(ret == 0){
			int j;
			for(j=0; j<i; j++){
				//close the pipe with the previous map workers
				safe_close(fd[2*j][1]);
				safe_close(fd[2*j+1][0]);	
			}
			//child process
			safe_close(fd[2*i][1]);
			safe_close(fd[2*i+1][0]);

			safe_dup2(fd[2*i][0],STDIN_FILENO);//redirect stdout
			safe_dup2(fd[2*i+1][1],STDOUT_FILENO);//redirect stdin

			safe_close(fd[2*i][0]);
			safe_close(fd[2*i+1][1]);

			execl("./mapworker", "mapworker", NULL);

		}else{
			//parent process
			safe_close(fd[2*i][0]);
			safe_close(fd[2*i+1][1]);
		}
	}
	return num;
}

