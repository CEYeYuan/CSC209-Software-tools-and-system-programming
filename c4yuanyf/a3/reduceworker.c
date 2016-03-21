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
		int sum = 0;
		LLValues *v_list = head->head_value;
		while(v_list != NULL){
			sum += atoi(v_list -> value);
			v_list = v_list->next;
		}
		fprintf(fp, "%s %d \n",head->key,sum );
		head = head->next;
	}		
	safe_fclose(fp);
	return 0;
}