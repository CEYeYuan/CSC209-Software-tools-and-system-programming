#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>

int count = 0;
int sec=1;

void handler(int code){
	printf("%s\n","SIGALRM Received" );
	printf("Read %d messages in %d second\n",count,sec);
	exit(0);
}

int main(int argc,char** argv){
	struct itimerval t;
	t.it_value.tv_sec=sec;
	t.it_value.tv_usec=0;

	setitimer(ITIMER_REAL,&t,NULL);
	struct sigaction newact;
	newact.sa_handler = handler;
	newact.sa_flags=0;
	sigemptyset(&newact.sa_mask);
	sigaction(SIGALRM,&newact,NULL);

	FILE *p = fopen("ok","rb");
	if(p == NULL){
		//should never happen
		return -1;
	}
	sigset_t set;
	sigaddset(&set,SIGALRM);
	while (1){
		int pos = random()%100;
		fseek(p,pos,SEEK_SET);
		char val; 
		fread(&val,1,1,p);
		//sigprocmask(SIG_BLOCK,&set,NULL);
		printf("Reading offset: %u val = %d \n",pos,val);	
		//sigprocmask(SIG_UNBLOCK,&set,NULL);
		count++;
	}
	return 0;
}