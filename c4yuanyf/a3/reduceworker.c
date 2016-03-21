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
		//fprintf(stderr,"child processing %s\n",pair.key);
		insert_into_keys(&head, pair); 
	}
	int pid = getpid();
	char name[30];
	sprintf(name, "%d.out", pid);
	FILE *fp = safe_fopen(name,"w");
	while(head != NULL){
		pair = reduce(head->key, head->head_value);
		fprintf(fp, "%s %s \n",head->key,pair.value);
		head = head->next;
	}		
	safe_fclose(fp);
	return 0;
}