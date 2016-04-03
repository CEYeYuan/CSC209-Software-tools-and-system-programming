#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "friends.h"

#define INPUT_BUFFER_SIZE 256
#define INPUT_ARG_MAX_NUM 12
#define DELIM " \n"


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

  if (listen(listenfd, 5) == -1) {
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
    int batch_mode = (argc == 2);
    char input[INPUT_BUFFER_SIZE];
    FILE *input_stream;

    // Create the heads of the empty data structure
    User *user_list = NULL;

    if (batch_mode) {
        input_stream = fopen(argv[1], "r");
        if (input_stream == NULL) {
            perror("Error opening file");
            exit(1);
        }
    } else {
        // interactive mode 
        input_stream = stdin;
    }

    /**********************************************************/
    //code from lab11
    int listenfd;
    int fd, nbytes;
    char buf[100];
    int inbuf; // how many bytes currently in buffer?
    int room; // how much room left in buffer?
    char *after; // pointer to position after the (valid) data in buf
    int where; // location of network newline

    struct sockaddr_in peer;
    socklen_t socklen;

    listenfd = setup();
    while (1) {
        socklen = sizeof(peer);
        // Note that we're passing in valid pointers for the second and third
        // arguments to accept here, so we can actually store and use client
        // information.
        if ((fd = accept(listenfd, (struct sockaddr *)&peer, &socklen)) < 0) {
          perror("accept");

        } else {
          safe_write(fd, "What is your user name?\n", strlen("What is your user name?\n") +1);
          // Receive messages
          inbuf = 0;          // buffer is empty; has no bytes
          room = sizeof(buf); // room == capacity of the whole buffer  printf("> ");
          after = buf;        // start writing at beginning of buf
          int is_inited = 0;      // the first cmd from the user would be the name 
          //bind that name with the fd number 


          while ((nbytes = read(fd, after, room)) > 0) {
            // Step 2: update inbuf (how many bytes were just added?)
            inbuf += nbytes;

            // Step 3: call find_network_newline, store result in variable "where"
            where = find_network_newline(buf, inbuf);
            
            if (where >= 0) { // OK. we have a full line

              // Step 4: output the full line, not including the "\r\n",
              // using print statement below.
              // Be sure to put a '\0' in the correct place first;
              // otherwise you'll get junk in the output.
              // (Replace the "\r\n" with appropriate characters so the 
              // message prints correctly to stdout.)
              buf[where] = '\n';
              buf[where+1] = '\0';
              
              //printf("processing command: %s", buf);
              // Note that we could have also used write to avoid having to
              // put the '\0' in the buffer. Try using write later!
              /********************************/
              if (batch_mode) {
                printf("%s", input);
              }

                char *cmd_argv[INPUT_ARG_MAX_NUM];
                int cmd_argc = tokenize(buf, cmd_argv);
                if(is_inited == 0){
                    //haven't created yet
                    strncpy(name, cmd_argv[0], 31);
                    name[31] = '\0';
                    is_inited = 1;
                    char *response = NULL;
                    switch (create_user(name, &user_list)) {
                        case 1:
                            response = "Welcome back.\r\nGo ahead and enter user commands>\r\n" ;
                            safe_write(fd, response, strlen(response) + 1);
                            safe_write(fd, "> ", strlen("> ") + 1);
                            break;
                        case 2:
                            response = "Username too long, truncated to 31 chars.\r\nGo ahead and enter user commands>\r\n" ;
                            safe_write(fd, response, strlen(response) + 1);
                            safe_write(fd, "> ", strlen("> ") + 1);
                            break;
                        case 0:
                            response = "Welcome.\r\nGo ahead and enter user commands>\r\n" ;
                            safe_write(fd, response, strlen(response) + 1);
                            safe_write(fd, "> ", strlen("> ") + 1);
                            break;
                    }

                }
                else{
                    if (cmd_argc > 0 && process_args(cmd_argc, cmd_argv, &user_list, name, fd) == -1) {
                        break; // can only reach if quit command was entered
                    }
                    safe_write(fd, "> ", strlen("> ") + 1);
                }
               
              /*********************************/
              
              
              // Step 5: update inbuf and remove the full line from the buffer
              // There might be stuff after the line, so don't just do inbuf = 0
              inbuf -= where+2;
              buf[where] = '\0';
              
              // You want to move the stuff after the full line to the beginning 
              // of the buffer.  A loop can do it, or you can use memmove.
              // memmove(destination, source, number_of_bytes)
              memmove(buf, buf+where+2, inbuf);
                
            }
            // Step 6: update room and after, in preparation for the next read
             room = sizeof(buf) - inbuf;
             after = inbuf + buf;
          }
          close(fd);
    }
  }
  /******************************************************/

 
    
    while (fgets(input, INPUT_BUFFER_SIZE, input_stream) != NULL) {
        // only echo the line in batch mode since in interactive mode the user
        // just typed the line
        if (batch_mode) {
            printf("%s", input);
        }

        char *cmd_argv[INPUT_ARG_MAX_NUM];
        int cmd_argc = tokenize(input, cmd_argv);

        if (cmd_argc > 0 && process_args(cmd_argc, cmd_argv, &user_list, name, fd) == -1) {
            break; // can only reach if quit command was entered
        }

        printf("> ");
    }

    if (batch_mode) {
        fclose(input_stream);
    }

    return 0;
 }
