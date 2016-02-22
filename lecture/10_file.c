#include <stdio.h>
/*
1.struct is passed by value, which means when we pass a struct into
a function, a copy of that struct is passed in
2.The sum of sizeof(members) of a struct is not equal to sizeof(struct)
since the byte alignment


FILE *fopen(const char *path,const char *mode)
int fseek(FILE* stream,long offset,int whence)
	SEEK_SET,SEEK_CUR,SEEK_END
size_t fread(void *ptr,size_t size,size_t nmemb,FILE *stream)
	ptr − This is the pointer to a block of memory with a minimum size of size*nmemb bytes.
	size − This is the size in bytes of each element to be read.
	nmemb − This is the number of elements, each one with a size of size bytes.
	stream − This is the pointer to a FILE object that specifies an input stream.

int fclose(FILE* stream)


*/
int main(int argc,char **argv){


	return 0;
}