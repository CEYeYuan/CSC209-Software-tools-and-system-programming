#include <stdio.h>

int get(){
	static i=1;
	return i++;
}
void main(){
	printf("%d %d %d\n",get(),get(),get() );
}