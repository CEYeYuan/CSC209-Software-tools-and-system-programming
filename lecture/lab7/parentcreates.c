#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>

int main(int argc, char **argv) {

	int i;
	int n;
	int num_kids;

	if(argc != 2) {
		fprintf(stderr, "Usage: forkloop <numkids>\n");
		exit(1);
	}
	//printf("pid = %d, ppid = %d\n", getpid(), getppid());

	num_kids = atoi(argv[1]);
	int status;

	for(i = 0; i < num_kids; i++) {
		n = fork();
		if(n < 0) {
			perror("fork");
			exit(1);
		}
		if(n == 0){
			//child : only prints the info and then return
			printf("pid = %d, ppid = %d, i = %d\n", getpid(), getppid(), i);
			return 0; 
		}
		//parent
		wait(&status);
		printf("pid = %d, ppid = %d, i = %d\n", getpid(), getppid(), i);
	}
	return 0;
}
