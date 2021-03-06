#include <stdio.h>
#include <stdlib.h>	
#include "mapreduce.h"
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "safe.h"
#include <sys/types.h>
#include <sys/wait.h>

int create_map_workers(int fd[][2], int num);
int create_reduce_workers(int fd[][2], int num);



int main(int argc,char** argv){
/*
	mapreduce -m 5 -r 3 -d texts.
*/
	int map_worker = 2;
	int reduce_worker = 2;
	char *dir = NULL;//not set yet
	int option;//setup the command line argument parsing
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
	int reduce_fd[reduce_worker][2];//fd for reduce worker

	
	safe_pipe(fd[0]);
	int ret = safe_fork();//fork process for ls
	if(ret == 0){
		//child process, close reading pipe and redirecion
		safe_close(fd[0][0]);
		safe_dup2(fd[0][1],STDOUT_FILENO);
		safe_close(fd[0][1]);
		execl("/bin/ls","ls",dir,NULL);
		perror("exec");
	}else{
		//parent process, read from the pipe
		//set up the pipe for the ls process
		safe_close(fd[0][1]);
		safe_dup2(fd[0][0], STDIN_FILENO);
		safe_close(fd[0][0]);
		//create processes for the map worker
		if (create_map_workers(map_fd,map_worker) != map_worker){
			//should never happen
			fprintf(stderr, "Error in creating map worker\n");
			return -1;
		}

		int count = 0;
		char *file_name = safe_malloc(sizeof(char) * MAX_FILENAME);
		while(scanf("%s",file_name) > 0){
			//reading file name while dispatching them
			//full name = path + file name
			int index = count % map_worker;
			char *full_name  = safe_malloc(sizeof(char)*MAX_FILENAME);
			strcat(full_name,dir);
			if(dir[strlen(dir)-1] != '/')
				strcat(full_name, "/");
			strcat(full_name,file_name);
			safe_write(map_fd[index*2][1], full_name, MAX_FILENAME);
			free(full_name);
			count++;
		}
		free(file_name);
		int i ;
		//make sure before we create reduce workers, all the pipes 
		//to map workers are properly closed
		for(i = 0; i < map_worker; i++){
			//no longer need these write pipes
			safe_close(map_fd[2*i][1]);
		}

		//create reduce workers
		Pair pair;
		if (create_reduce_workers(reduce_fd,reduce_worker) != reduce_worker){
			//should never happen
			fprintf(stderr, "Error in creating reduce worker\n");
			return -1;
		}

		for(i = 0; i < map_worker; i++){
			//read key-value pairs from map worker and dispatch them
			while(safe_read(map_fd[2*i+1][0], &pair, sizeof(pair)) > 0){
				char c = pair.key[0];
				int index = c % reduce_worker;
				//dispatch the key according to their first character
				safe_write(reduce_fd[index][1],&pair, sizeof(pair));
			}
			//done reading from that child
			safe_close(map_fd[2*i+1][0]);
		}

		for(i = 0; i < reduce_worker; i++){
			//close the writing pipe to the reduce worker
			safe_close(reduce_fd[i][1]);
		}
	}
	
	printf("wait %d process to complete \n",map_worker+reduce_worker);
	int i;
	for(i = 0; i < 1 + map_worker + reduce_worker; i++){
		int status;
		wait(&status);
	}	
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

			safe_dup2(fd[2*i][0],STDIN_FILENO);//redirect stdin
			safe_dup2(fd[2*i+1][1],STDOUT_FILENO);//redirect stdout

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

int create_reduce_workers(int fd[][2],int num){
	int i;
	for(i = 0;i < num;i++){
		//the pipe is used for sending data to a reduce worker
		safe_pipe(fd[i]);
		int ret = safe_fork();
		if(ret == 0){
			//child process
			int j;
			for(j=0; j<i; j++){
				//close the unused inherited pipes
				safe_close(fd[j][1]);
			}
			safe_close(fd[i][1]);
			safe_dup2(fd[i][0],STDIN_FILENO);//redirect stdin
			safe_close(fd[i][0]);
			execl("./reduceworker", "reduceworker", NULL);

		}else{
			//parent process
			safe_close(fd[i][0]);
		}
	}
	return num;
}
