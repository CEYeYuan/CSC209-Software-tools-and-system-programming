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
	//read the key-value pair from the master process
	//process it and then write to the file
	while(safe_read(STDIN_FILENO,&pair,sizeof(pair)) > 0){
		insert_into_keys(&head, pair); 
	}
	int pid = getpid();
	char name[MAX_FILENAME];
	sprintf(name, "%d.out", pid);
	FILE *fp = safe_fopen(name,"wb");
	LLKeyValues *cur = head;
	while(cur != NULL){
		pair = reduce(cur->key, cur->head_value);
		safe_fwrite(cur->key, strlen(cur->key)+1, 1, fp);
		safe_fwrite(pair.value, strlen(pair.value)+1, 1, fp);
		cur = cur->next;
	}	
	free_key_values_list(head);
	safe_fclose(fp);
	return 0;
}