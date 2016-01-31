#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main(int argc,char **argv)
{
    int i,j,inited,done,line;
    char tmp[11][256];//buffer for each line
    char max[11][256];//buffer for the process with max usage
    strcpy(max[2],"-1");
    strcpy(max[3],"-1");
    char buffer[256];
    inited=done=line=0;
    
    if(argc<=1||argc>=4){
    	printf("%s\n","too few/much argument" );
    	printf("%s\n","Correct usage: hogs -m/-u username" );
    	return -1;//error
    }

    while(1){
    	if(done)
			break;
	    //we only wants the useful data, so skip line 1
	   	if(line==0){
	   		for(j=0;j<11;j++)
	   			scanf("%s",buffer);
		}
	    
	    else{
		    for(j=0;j<11;j++){
		   		int num_char;
		   		num_char=scanf("%s",tmp[j]);
		   		if(num_char<=0){
		   			done=1;
		    		break;
		    	}
		   	}

		   	if(argc==2){
		   		if(strtol(tmp[2],NULL,10)>strtol(max[2],NULL,10)&&strcmp(tmp[0],argv[1])==0){
					inited=1;
	   				for(j=0;j<11;j++){
	   					strcpy(max[j],tmp[j]);
		   			}
	    		}
	    	}	
		    if(argc==3){
		   		if(strcmp(argv[1],"-m")==0&&strcmp(tmp[0],argv[2])==0){
		   			if(strtof(tmp[3],NULL)>strtof(max[3],NULL)){
		   				inited=1;
			   			for(j=0;j<11;j++){
			   				strcpy(max[j],tmp[j]);
		   				}
		   			}
	    		}

	    		if(strcmp(argv[1],"-c")==0&&strcmp(tmp[0],argv[2])==0){
	    			if(strtof(tmp[2],NULL)>strtof(max[2],NULL)){
	    				inited=1;
		   				for(j=0;j<11;j++){
		   					strcpy(max[j],tmp[j]);
			   			}
	    			}
	    		}

		    }
	    }
		line++;
   } 
   if(inited==1){
   		if(argc==3&&strcmp(argv[1],"m")==0)
   			printf("%s\t%.1f\t%s\n", max[1],strtof(max[3],NULL),max[10]);
   		else
   			printf("%s\t%.1f\t%s\n", max[1],strtof(max[2],NULL),max[10]);
    }
   
}