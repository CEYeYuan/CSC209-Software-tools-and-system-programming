#include <stdio.h>
void main(){
	int x=10;
	int max=sizeof(int)*8;
	int i,count;
	count=0;
	for(i=0;i<max;i++){
		if(x&1==1)
			count++;
		x=x>>1;
	}
	printf("count=%d\n",count);
}