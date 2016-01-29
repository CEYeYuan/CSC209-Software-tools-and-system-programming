#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	char str1[20]="hello world";//array definition: mutable
	str1[0]='w';
	printf("%s   %p\n",str1,str1);//	wello world   on the stack

	char *str2="hello world";	//string literal; immutable
	printf("%s %p\n",str2,str2);//global region
	char *str3="how are you";
	printf("%s %p\n",str3,str3);//different address
	str2=str3;
	printf("%s %p\n",str2,str2);
	//str2[4]='b';//segmentation fault

	
	char *str=malloc(sizeof(char)*20);
	printf("%s %p\n",str,str);//heap
	*str='a';
	*(str+1)='b';
	printf("%s %p\n",str,str );
	char *tmp=str;
	str="another str";
	printf("%s  %p\n",tmp,tmp);//old address
	printf("%s %p\n",str,str);//new address

}