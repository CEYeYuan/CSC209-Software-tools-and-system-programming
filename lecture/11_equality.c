#include <stdio.h>
#include <string.h>
int main(int argc, char ** argv){
	
	char *s1 = "hello word";
	char *s2 = s1;
	char s3[20];
	strcpy(s3,s1);
	if(s1 == s2)
		printf(" ptr equal \n");//true
	if(s1 == s3)
		printf("%s\n","string equal" );

	/*
	The compiler sees a comparison with a char* on either side, so it does a pointer 
	comparison (which compares the addresses stored in the pointers)
	*/

	return 0;
}