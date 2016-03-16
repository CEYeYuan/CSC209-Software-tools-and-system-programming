#include <stdio.h>
int main(int argc, char ** argv){

	if(argc != 2){
		printf("%s\n","Enter the file name" );
		return -2;
	}
	
	FILE *file = fopen(argv[1],"wb");
	if(file == NULL)
		return -1;//should not happen

	int i;
	for(i = 0;i < 100; i++){
		int rd = random()%100;
		fwrite(&rd,1,1,file);
	}
	return 0;
}