#include <stdio.h>
#include <stdlib.h>	
#include "mapreduce.h"
#include <unistd.h>
#include "safe.h"
#include "linkedlist.h"
#include <string.h>
#include <ctype.h>


Pair*  map1(const char *chunk, int outfd) {
	Pair *ret = malloc(READSIZE*sizeof(Pair));
	int count = 0;
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
                strcpy(ret[count].key,pair.key);
                strcpy(ret[count].value,pair.value);
                count++;
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
         strcpy(ret[count].key,pair.key);
         strcpy(ret[count].value,pair.value);
         count++;
    }
    return ret;
}


int main(){
	char *p[3];
	p[0] = "texts/1ws0610.txt";
	p[1] ="texts/1ws1711.txt";
	p[2] ="texts/1ws2410.txt";
	p[3] ="texts/1ws4111.txt";
	int i = 0;
	LLKeyValues *head = NULL;
	for(i = 0; i < 3;i++){
		FILE *file = safe_fopen(p[i],"r");
		char *chunk = malloc(sizeof(char)*READSIZE);
		while(fread(chunk,READSIZE-1,1,file) > 0){
			chunk[READSIZE-1] = '\0';
			Pair *p = map1(chunk,STDIN_FILENO);
			while(p != NULL && p->key != NULL){
				//fprintf(stderr,"child processing %s\n",pair.key);
					insert_into_keys(&head, *p);
					p +=1; 
			}
		}
		safe_fclose(file);
	}
	
	FILE *fp = safe_fopen("reference","w");
	while(head != NULL){
		Pair pair = reduce(head->key, head->head_value);
		fprintf(fp, "%s %s \n",head->key,pair.value);
		head = head->next;
	}		
	safe_fclose(fp);
	return 0;
}

