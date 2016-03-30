#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(){
	int fd[2];
	pipe(fd);
	printf("read = %d write = %d\n",fd[0],fd[1]);
	while(1){
		char *p = malloc(sizeof(100));
		read(fd[0], p, 100);
		printf("%s\n",p );
	}
	return 0;
}