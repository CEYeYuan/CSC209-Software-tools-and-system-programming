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

	/*
	strcpy and strncpy
	The safe versions of the unsafe functions often add an n somewhere in their name; this n indicates that 
	there is a new n parameter added to the function to control how much activity the function is allowed to 
	do before stopping. In the case of strcpy, we have the safe function strncpy. The new parameter n indicates
	the maximum number of characters that s1 can hold, and therefore the maximum number of characters,including
	any null character, that can be copied from s2 into s1.
	Unfortunately, this code is also unsafe. The reason has to do with a tricky fact about strcpy: it is not 
	guaranteed to add a null terminator, unless it finds one in the first n characters of s2.
	
	char s1[5];
    char s2[32] = "University of";

    // This is unsafe because s1 may not have enough space
    // to hold all the characters copied from s2.
    //strcpy(s1, s2);

    // This doesn't necessarily null-terminate s1 if there isn't space.
    strncpy(s1, s2, sizeof(s1));
    // So we explicitly terminate s1 by setting a null-terminator.
    s1[4] = '\0';



	strcat and strncat
	Like strcpy in the previous video, strcat is an unsafe function. s1 may not have enough space to store its
	contents as well as s3's contents. Similar to the case for strcpy, we have an n function called strncat 
	that is a safe version of strcat.
	The new n parameter indicates the maximum number of characters, not including the null terminator, that 
	should be copied from s2 to the end of s1. Unlike strncpy, strncat always adds a null terminator to s1.
	
	char s1[30];
    char s2[14] = "University of";
    char s3[15] = " C Programming";
    strcpy(s1, s2);
    strncat(s1, s3, sizeof(s1) - strlen(s1) - 1);30- # of char(not including NULL)-null 
    printf("%s\n", s1);

	*/

	return 0;
}