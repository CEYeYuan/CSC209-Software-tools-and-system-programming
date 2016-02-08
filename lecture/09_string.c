#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	char first[20]="monday";
	printf("%s\n",first );
	char *second="tuesday";
	char *third=malloc(sizeof(char)*20);
	strcpy(third,"wednesday");
	return 0;
	
}