#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

/* Read a user id and password from standard input, 
   - create a new process to run the validate program
   - send 'validate' the user id and password on a pipe, 
   - print a message 
        "Password verified" if the user id and password matched, 
        "Invalid password", or 
        "No such user"
     depending on the return value of 'validate'.
*/


int main(void) {
    char userid[10];
    char password[10];

    /* Read a user id and password from stdin */
    printf("User id:\n");
    scanf("%s", userid);
    printf("Password:\n");
    scanf("%s", password);

    /*Your code here*/
    int fd[2] ;
    if (pipe(fd) != 0){
    	perror("Error");
    	//should not happen
    }
    int ret = fork();
    if(ret == 0){
    	//child process, would run validate
    	close(fd[1]);//don't need write channel	
    	if(dup2(fd[0],STDIN_FILENO) == -1){
    		//overwrite the stdin channel
    		perror("dup2");
    	}
    	close(fd[0]);
    	execlp("./validate","./validate",NULL);
    	perror("exec");
    }else{
    	//parent process
    	close(fd[0]);//do not need t read
    	//make sure buffer won't overflow
    	if(userid[9] != '\0' && userid[9] != '\n')
    		userid[9] = '\0';
    	if(password[9] != '\0' && password[9] != '\n')
    		password[9] = '\0';
    	write(fd[1], userid, 10);
    	write(fd[1],password,10);
    	close(fd[1]);
    	int status;
    	wait(&status);
    	if(WIFEXITED(status) != 0){
    		if(WEXITSTATUS(status) == 0)
    			printf("%s\n","Password verified" );
    		if(WEXITSTATUS(status) == 2)
    			printf("%s\n","Invalid password" );
    		if(WEXITSTATUS(status) == 3)
    			printf("%s\n","No such user" );
    	}
    	else
    		printf("%s\n","Error" );//should never happen
    	
    }

    
    return 0;
}
