#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
 
int main(int argc,char **argv)
{
    int i,j;
    char *tmp[11];//buffer for each line
    char *max[11];//buffer for the process with max usage
    char *flag[11];
    char first_line[128];//buffer for the first line. Useless since no actual data
    int done=0;

    for(i=0;i<11;i++){
    	tmp[i]=malloc(sizeof(char)*128);
    	max[i]=malloc(sizeof(char)*128);
    }

    int line=0;
    while(1){
    	if(done)
	    	break;
    	//we only wants the useful data, so skip line 1
    	if(line==0){
    	//	printf("here\n");
    		for(j=0;j<11;j++)
    			scanf("%s",max[j]);
    	}
    	else{
    	
	    	for(j=0;j<11;j++){
	    		int num_char;
	    		num_char=scanf("%s",tmp[j]);
	    		if(num_char<=0){
	    			done=1;
	    			break;
	    		}
	    		//printf("argc=%d argv[1]=%s\n",argc,argv[1] );
	    		if(argc==3){
	    		//	printf("arv[1]=%s\n",argv[1] );
	    			if(argv[1]=="-m")
	    				printf("%s\n","equal" );
	    			if(tmp[0]==argv[2]&&strtol(tmp[3],NULL,10)>strtol(max[3],NULL,10)){
	    				for(j=0;j<11;j++){
	    					flag[j]=tmp[j];
	    					tmp[j]=max[j];
	    					max[j]=flag[j];
	    				}
	    			}
	    		}
	    		/*if((argc==2)||(argc==3&&argv[2]=="-c")){
	    			if(strtol(tmp[2])>strtol(max[2])){
	    				for(j=0;j<11;j++){
	    					max[j]=tmp[j];
	    				}
	    			}
	    		}*/
	    		//printf("%s ",tmp[j]);
	    	}
	    	
	    	//printf("\n");
    	}
    	line++;
    } 
    /*for(j=0;j<11;j++){
	    printf("%s ",max[j]);
	} */     
}