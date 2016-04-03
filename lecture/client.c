#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>    
#include <arpa/inet.h>

#define PORT 30000

int main(int argc, char **argv) { 

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
   
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr)< 0) {
        perror("client: inet_pton");
        close(sock_fd);
        exit(1);
    }

    if (connect(sock_fd, (struct sockaddr *) &server, sizeof(server)) == -1) {
        perror("client: connect");
        close(sock_fd);
        exit(1);
    }

    printf("I'm connected\n");
    write(sock_fd, "Hello Internet\r\n", 16); 

    //char buf[128];
    // We commented out the line above and replaced it with this
    // to point out that if buf doesn't happen to be filled with 0's
    // the wrong choices below are a problem.
    char buf[128] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

    int numread = read(sock_fd, buf, sizeof(buf) - 1);

    //  not adding a null-terminator means we could write beyond the end of buf
    //  buf[strlen(buf)] = '\0';    Don't do this. It doesn't help.
    //  buf[127] = '0';    We demonstrated that this was a bad idea as well.
    buf[numread] = '\0';

    printf("server wrote %s\n", buf);

    close(sock_fd);
    return 0;
}