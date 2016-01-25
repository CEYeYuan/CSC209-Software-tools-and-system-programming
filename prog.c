#include <stdio.h>
int main(int argc,char **argv){
	if(argc>=2){
		printf("%s\n",argv[1]);
	}
	else{
		printf("%s\n","Not enough arguments" );
	}
	return 0;
}