#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handler(int code){
	printf("%s\n","SIGALRM Received" );
	exit(0);
}
int main(int argc,char** argv){

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
	while (1){
		int pos = random()%100;
		fseek(p,pos,SEEK_SET);
		char val; 
		fread(&val,1,1,p);
		printf("pos = %u val = %d \n",pos,val);
	}
	return 0;
}