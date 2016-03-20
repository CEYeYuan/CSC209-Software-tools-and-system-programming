#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void safe_close(int fd){
	int ret = close(fd);
	if(ret == -1){
		perror("close");
		exit(-1);
	}
}

void safe_dup2(int p, int q){
	int ret = dup2(p,q);
	if(ret == -1){
		perror("dup2");
		exit(-1);
	}
}

int safe_read(int fd, void *buf, int count){
	int ret = read(fd,buf,count);
	if(ret == -1){
		perror("read");
		exit(-1);
	}
	return ret;
}

int safe_write(int fd, void *buf, int count){
	int ret = write(fd,buf,count);
	if(ret == -1){
		perror("write");
		exit(-1);
	}
	return ret;
}


int safe_fork(){
	int ret = fork();
	if(ret == -1){
		perror("fork");
		exit(-1);
	}
	return ret;
}

void safe_pipe(int *fd){
	int ret = pipe(fd);
	if(ret == -1){
		perror("pipe");
		exit(-1);
	}
}

