#include <stdlib.h>
int sum(int arr[3]){
	return arr[0]+arr[1]+arr[2000];
}
int main(){
	int a[3]={1,2,3};
	int *b = malloc(sizeof(int)*2);
	b[0] = 1;
	b[1] = 2;
	b[2000] = 3;
	//b[2222] =4;
	sum(a);

	return 0;
}