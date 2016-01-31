#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc,char **argv){
//string: a null terminated char array
	char text[6];
	text[0]='h';
	text[1]='e';	
	text[2]='l';
	text[3]='l';
	text[4]='o';
	text[5]='\0';
	printf("%s\n",text); //hello

	//text[20]={'h','e','l','l','o','\0'};all the element after index 5 is NULL
	//text[20]="hello";	all the element after index 5 is NULL
	//text[]="hello";	sizeof(text)=6;

	char str1[20]="hello";
	char str2[20]="good";
	strcpy(str1,str2);
	printf("%s\n", str1);

	if(str1=="good")
		printf("%s\n","equal" );//nothing 

	char str3[10]="good";
	if(strcmp(str1,str3)==0)
		printf("%s\n","equal" );//equal


	char num[10];
	printf("%ld \n",strtol(num,NULL,10));

	return 0;
}