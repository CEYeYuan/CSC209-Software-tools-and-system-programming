#include <stdio.h>
#include <unistd.h>
int main(){
	int x ;
	scanf("%d",&x);
	char p[100];
	scanf("%s",p);
	int ret = write(x,p,100);
	printf("ret == %d\n",ret);
	return 0;
}