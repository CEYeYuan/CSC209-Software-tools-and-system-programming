#include <stdio.h>
#include <string.h>
void main(){
	char *p="hello world";
	char p2[10];
	strcpy(p2,"hello");
	printf("sizeof(p)=%d,sizeof(p2)=%d\n",sizeof(p),sizeof(p2) );
}