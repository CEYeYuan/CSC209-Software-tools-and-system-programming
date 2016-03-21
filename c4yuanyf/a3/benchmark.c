#include <stdio.h>
#include <stdlib.h>	
#include "mapreduce.h"
#include <unistd.h>
#include "safe.h"
#include "linkedlist.h"
#include <string.h>
#include <ctype.h>

void  map1(const char *chunk, LLKeyValues **head) {
    Pair pair = {"", "1"};
    int index = 0;
    const char *cptr = chunk;

    // Get rid of any initial whitespace
    while (*cptr != '\0' && isspace(*cptr)) {
        cptr++;
    }

    while (*cptr != '\0') {
        // If we have reached the end of the word then terminate and emit.
        if (isspace(*cptr)) {
            if (index == 0) { // don't emit empty strings.
                cptr++;
                continue;
            } else {
                pair.key[index] = '\0';
               // write(outfd, &pair, sizeof(Pair));
              	insert_into_keys(head,pair);
                while (isspace(*cptr)) {
                    cptr++;
                }
                index = 0;
            }
        // ignore punctuation (This is a simplification.)
        } else if (ispunct(*cptr)) {
            cptr++;
        // otherwise add the character to our current word.
        } else {
            pair.key[index] = tolower(*cptr);
            cptr++;
            index++;
        }
    }

    // write the last word
    pair.key[index] = '\0';
    if (index > 0) {
        //write(outfd, &pair, sizeof(Pair));
        insert_into_keys(head,pair);
    }
}


int main(){
	char *p[3];
	p[0] = "texts/1ws0610.txt";
	p[1] ="texts/1ws1711.txt";
	p[2] ="texts/1ws2410.txt";
	p[3] ="texts/1ws4111.txt";
	int i = 0;
	LLKeyValues *head = NULL;
	for(i = 0; i < 4;i++){
		FILE *file = safe_fopen(p[i],"r");
		char *chunk = malloc(sizeof(char)*READSIZE);
		while(fread(chunk,READSIZE-1,1,file) > 0){
			chunk[READSIZE-1] = '\0';
			map1(chunk,&head);
		}
		safe_fclose(file);
	}
	
	FILE *fp = safe_fopen("reference","wb");
	while(head != NULL){
		Pair pair = reduce(head->key, head->head_value);
		fwrite(head->key, strlen(head->key)+1, 1, fp);
		fwrite(pair.value, strlen(pair.value)+1, 1, fp);
		head = head->next;
	}		
	safe_fclose(fp);
	return 0;
}

