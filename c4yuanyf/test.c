#include <stdio.h>
#include <string.h>

int main(){
	char buf[100];
	char *p1 = "hello";
	printf("%s\r\n",p1);

	buf[6]='s';
	int ret = snprintf(buf, 6, "%s\n",p1);
	printf("%s  %d\n",buf,ret);
	return 0;
}