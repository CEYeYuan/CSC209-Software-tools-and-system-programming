#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>



char *arr;
void sing(int code){
	printf("Happy Birthday to %s\n",arr);
	sleep(10);
}

void dance(int code){
	printf("Happy Birthday to %s\n",arr);
	sleep(10);
}

int main(int argc, char **argv){
	arr=argv[1];
	struct sigaction act;
	act.sa_handler=sing;
	act.sa_flags=0;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask,SIGINT);
	//the set of signal will be blocked	
	sigaction(SIGUSR1,&act,NULL);


	struct sigaction act2;
	act2.sa_handler=sing;
	act2.sa_flags=0;
	sigemptyset(&act2.sa_mask);
	sigaddset(&act2.sa_mask,SIGINT);
	//the set of signal will be blocked	
	sigaction(SIGUSR2,&act2,NULL);

	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset,SIGUSR2);

	while(1){
		fprintf(stderr, "Who wants to dance \n");
		sleep(10);
		sigprocmask(SIG_UNBLOCK,&sigset,NULL);
		//print to stderr is not bufferred while stdout is bufferred until 
		//encounter a new line or the os choose to flush 
		fprintf(stderr, "I'm too tired to dance \n");
		sigprocmask(SIG_BLOCK,&sigset,NULL);
		sleep(10);
	}
	return 0;
}