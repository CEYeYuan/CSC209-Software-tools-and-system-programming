#include "mapreduce.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "safe.h"
#include "linkedlist.h"
#include <string.h>

int main(){
	Pair pair;
	LLKeyValues *head = NULL;
	while(safe_read(STDIN_FILENO,&pair,sizeof(pair)) > 0){
		insert_into_keys(&head, pair); 
	}
	int pid = getpid();
	char name[30];
	sprintf(name, "%d.out", pid);
	FILE *fp = safe_fopen(name,"wb");
	while(head != NULL){
		pair = reduce(head->key, head->head_value);
		fwrite(head->key, strlen(head->key)+1, 1, fp);
		fwrite(pair.value, strlen(pair.value)+1, 1, fp);
		head = head->next;
	}		
	safe_fclose(fp);
	return 0;
}