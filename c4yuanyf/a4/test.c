#include <stdio.h>
#include <string.h>

int main(){
	char buf[100];
	char *p1 = "hello";
	char *p2 = " world";
	snprintf(buf, strlen(p1)+1, "%s",p1);
	snprintf(buf+6, strlen(p2)+1,"%s", p2);
	printf("%s\n",buf);
	printf("%s\n",buf+6);
	return 0;
}