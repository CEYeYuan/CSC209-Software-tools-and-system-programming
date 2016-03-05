#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc, char **argv) {

	int i;
	int n;
	int num_kids;

	if(argc != 2) {
		fprintf(stderr, "Usage: forkloop <numkids>\n");
		exit(1);
	}

	num_kids = atoi(argv[1]);

	for(i = 0; i < num_kids; i++) {
		n = fork();
		if(n < 0) {
			perror("fork");
			exit(1);
		}
		if(n > 0){
			//parent process
			//printf("pid = %d, ppid = %d, i = %d\n", getpid(), getppid(), i);
			int status;
			wait(&status);
			break;
		}
		//only child process can reach here, continue produce more child process
		//ppid should be the pid of the main process
 		printf("pid = %d, ppid = %d, i = %d\n", getpid(), getppid(), i);
	}

	return 0;
}
