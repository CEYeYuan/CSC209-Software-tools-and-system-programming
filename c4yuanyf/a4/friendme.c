#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "friends.h"

#define INPUT_BUFFER_SIZE 256
#define INPUT_ARG_MAX_NUM 12
#define DELIM " "


/* 
 * Print a formatted error message to stderr.
 */
void error(char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
}

void safe_write(int fd, const void *buf, int count){
  int ret = write(fd, buf, count);
  if(ret == -1){
    perror("write:");
    exit(-1);
  }
}

int safe_read(int fd, void *buf, int count){
  int ret = read(fd, buf, count);
  if(ret == -1){
    perror("read:");
    exit(-1);
  }
  printf("buffer: %s\n", (char *)buf);
  return ret;
}

/* 
 * Read and process commands
 * Return:  -1 for quit command
 *          0 otherwise
 */
int process_args(int cmd_argc, char **cmd_argv, User **user_list_ptr, char *name, int fd) {
    User *user_list = *user_list_ptr;
    char *response = NULL;
    int len = 0;

    if (cmd_argc <= 0) {
        return 0;
    } else if (strcmp(cmd_argv[0], "quit") == 0 && cmd_argc == 1) {
        return -1;
    }  else if (strcmp(cmd_argv[0], "list_users") == 0 && cmd_argc == 1) {
        char * buf =list_users(user_list);
        safe_write(fd, buf, strlen(buf)+1);
        free(buf);

    } else if (strcmp(cmd_argv[0], "make_friends") == 0 && cmd_argc == 2) {
        switch (make_friends(cmd_argv[1], name, user_list)) {
            case 0:
                 len = strlen("You are now friends with ") + strlen(cmd_argv[1]) + 3;
                 // 3 for "\r\n" and null terminator
                 response = malloc(sizeof(char) * len);
                 snprintf(response, len, "You are now friends with %s\r\n", cmd_argv[1]);
                 safe_write(fd, response, strlen(response)+1);
                 free(response);
                 break;
            case 1:
                response = "users are already friends\r\n";
                safe_write(fd, response, strlen(response)+1);
                break;
            case 2:
                response = "at least one user you entered has the max number of friends\r\n";
                safe_write(fd, response, strlen(response)+1);
                break;
            case 3:
                //should never happen
                //error("you must enter two different users");
                break;
            case 4:
                response = "the user you entered does not exist\r\n";
                safe_write(fd, response, strlen(response)+1);
                break;
        }
    } else if (strcmp(cmd_argv[0], "post") == 0 && cmd_argc >= 3) {
        // first determine how long a string we need
        int space_needed = 0;
        for (int i = 2; i < cmd_argc; i++) {
            space_needed += strlen(cmd_argv[i]) + 1;
        }

        // allocate the space
        char *contents = malloc(space_needed);
        if (contents == NULL) {
            perror("malloc");
            exit(1);
        }

        // copy in the bits to make a single string
        strcpy(contents, cmd_argv[2]);
        for (int i = 4; i < cmd_argc; i++) {
            strcat(contents, " ");
            strcat(contents, cmd_argv[i]);
        }

        User *author = find_user(name, user_list);
        User *target = find_user(cmd_argv[1], user_list);
        switch (make_post(author, target, contents)) {
            case 1:
                response = "You can only post to your friends\r\n";
                safe_write(fd, response, strlen(response)+1);
                break;
            case 2:
                response = "the user you entered does not exist\r\n";
                safe_write(fd, response, strlen(response)+1);
                break;
        }
    } else if (strcmp(cmd_argv[0], "profile") == 0 && cmd_argc == 2) {
        User *user = find_user(cmd_argv[1], user_list);
        if (print_user(user) == NULL) {
            response = "user not found\r\n";
            safe_write(fd, response, strlen(response)+1);
        }else{
            char *buf = print_user(user);
            safe_write(fd, buf, strlen(buf)+1);
            free(buf);
        }
    } else {
        response = "Incorrect syntax\r\n";
        safe_write(fd, response, strlen(response)+1);
    }
    return 0;
}


/*
 * Tokenize the string stored in cmd.
 * Return the number of tokens, and store the tokens in cmd_argv.
 */
int tokenize(char *cmd, char **cmd_argv) {
    int cmd_argc = 0;
    char *next_token = strtok(cmd, DELIM);    
    while (next_token != NULL) {
        if (cmd_argc >= INPUT_ARG_MAX_NUM - 1) {
            error("Too many arguments!");
            cmd_argc = 0;
            break;
        }
        cmd_argv[cmd_argc] = next_token;
        cmd_argc++;
        next_token = strtok(NULL, DELIM);
    }

    return cmd_argc;
}
/*******************************************************/
//code from lab 11
int setup(void) {
  int on = 1, status;
  struct sockaddr_in self;
  int listenfd;
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  // Make sure we can reuse the port immediately after the
  // server terminates.
  status = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                      (const char *) &on, sizeof(on));
  if(status == -1) {
    perror("setsockopt -- REUSEADDR");
  }

  self.sin_family = AF_INET;
  self.sin_addr.s_addr = INADDR_ANY;
  self.sin_port = htons(PORT);
  memset(&self.sin_zero, 0, sizeof(self.sin_zero));  // Initialize sin_zero to 0

  printf("Listening on %d\n", PORT);

  if (bind(listenfd, (struct sockaddr *)&self, sizeof(self)) == -1) {
    perror("bind"); // probably means port is in use
    exit(1);
  }

  if (listen(listenfd, 20) == -1) {
    perror("listen");
    exit(1);
  }
  return listenfd;
}

/*Search the first inbuf characters of buf for a network newline ("\r\n").
  Return the location of the '\r' if the network newline is found,
  or -1 otherwise.
  Definitely do not use strchr or any other string function in here. (Why not?)
*/

int find_network_newline(const char *buf, int inbuf) {
  // Step 1: write this function
  int i ;
  for(i = 0; i <= inbuf-2; i++){
    if(buf[i] != '\r')
        continue;
    if(buf[i+1] == '\n')
        return i;
  }
  return -1; // return the location of '\r' if found
}
/***********************************************************/
//code end from lab11

int main(int argc, char* argv[]) {
    char name[32];
    // Create the heads of the empty data structure
    User *user_list = NULL;

    /**********************************************************/
    //code from lab11
    int listenfd;
    int fd, nbytes;
    

    struct sockaddr_in peer;
    socklen_t socklen;
    List *head = NULL;// the list-map for the fd and name
    listenfd = setup();
    add_fd(listenfd, &head);
    while (1) {

        fd_set set;
        build_fdset(&set, head);
        int numfd = find_max_fd(head) + 1;
         //select always remove fd from the set, never add more in
        if (select(numfd, &set, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }

        // If there is data from listen_fd, accept it

        if (FD_ISSET(listenfd, &set)) {
            socklen = sizeof(peer);
            // Note that we're passing in valid pointers for the second and third
            // arguments to accept here, so we can actually store and use client
            // information.
            if ((fd = accept(listenfd, (struct sockaddr *)&peer, &socklen)) < 0) {
                perror("accept");
                close(fd);
                exit(1);
            } 
            else {        
              add_fd(fd, &head);//add it to the map structure
              safe_write(fd, "What is your user name?\n", strlen("What is your user name?\n") +1);
            }
        }

        List *cur = head;
        while (cur != NULL && cur->fd > 0){
            if (FD_ISSET(cur->fd, &set) && cur->fd != listenfd) {
                nbytes = safe_read(cur->fd, cur->after, cur->room) ;
                if (nbytes < 0) {
                    perror("read");
                } 
                else if (nbytes == 0) {
                    //if read returns 0, we know that users is disconnected
                    invalid(cur->fd,head);
                    printf("bye\n");
                    close(fd);
                }
                else {
                    // Receive messages
                    // Step 2: update inbuf (how many bytes were just added?)
                    cur->inbuf += nbytes;
                    // Step 3: call find_network_newline, store result in variable "where"
                    cur->where = find_network_newline(cur->buf, cur->inbuf);
            
                    if (cur->where >= 0) { // OK. we have a full line
                        cur->buf[cur->where] = '\0';
                        cur->buf[cur->where+1] = '\0';
                        char *tmp = malloc(strlen(cur->buf) + 1);
                        strncpy(tmp, cur->buf, strlen(cur->buf) + 1);
                        char *cmd_argv[INPUT_ARG_MAX_NUM];
                        int cmd_argc = tokenize(tmp, cmd_argv);
                        if(cur->inited == 0){
                            //haven't created yet
                            strncpy(name, cmd_argv[0], 31);
                            name[31] = '\0';
                            set_name(cur->fd, head, name);
                            char *response = NULL;
                            if(nbytes >= 31+2){
                                //31 for the characters, 2 for the network newline
                                response = "Username too long, truncated to 31 chars.\r\n" ;
                                safe_write(cur->fd, response, strlen(response) + 1);
                                safe_write(cur->fd, "> ", strlen("> ") + 1); 
                            }
                            switch (create_user(name, &user_list)) {
                                case 1:
                                    response = "Welcome back.Go ahead and enter user commands\r\n" ;
                                    safe_write(cur->fd, response, strlen(response) + 1);
                                    safe_write(cur->fd, "> ", strlen("> ") + 1);
                                    break;
                                case 2:
                                    //should never happen since we already handle the string length
                                    break;
                                case 0:
                                    response = "Welcome.Go ahead and enter user commands\r\n" ;
                                    safe_write(cur->fd, response, strlen(response) + 1);
                                    safe_write(cur->fd, "> ", strlen("> ") + 1);
                                    break;
                            }

                        }
                        else{
                            //printf("read %d bytes :%s\n",nbytes, cmd_argv[0]);
                            if (cmd_argc > 0 && process_args(cmd_argc, cmd_argv, &user_list, cur->name, cur->fd) == -1) {
                                invalid(cur->fd,head);
                                printf("bye\n");
                                close(fd); // can only reach if quit command was entered
                            }
                            safe_write(cur->fd, "> ", strlen("> ") + 1);
                        }
                     
                        // Step 5: update inbuf and remove the full line from the buffer
                        cur->inbuf -= cur->where+2;
                        cur->buf[cur->where] = '\0';
                        
                        // You want to move the stuff after the full line to the beginning 
                        // of the buffer.  
                        memmove(cur->buf, (cur->buf)+cur->where+2, cur->inbuf);
                    
                    }
                    // Step 6: update room and after, in preparation for the next read
                    cur->room = sizeof(cur->buf) - cur->inbuf;
                    cur->after = cur->inbuf + cur->buf;      
                }
           }
           cur = cur->next;
        }
  }
    return 0;
  }
