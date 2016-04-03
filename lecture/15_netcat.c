#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <stdio.h>

#define MAXSIZE 256

int main(int argc, char **argv) {
    int r;
    char buf[MAXSIZE];

    // create the socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("server: socket");
        exit(1);
    }

    // set up the sockaddr_in struct for connecting
    struct sockaddr_in peer;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(strtol(argv[2], NULL, 10));

    //pton : presentation to network: 0.0.0.0 -> network
    if (inet_pton(AF_INET, argv[1], &peer.sin_addr) < 0) { 
        perror("inet_pton");
        exit(1);
    }

    // connect the socket
    if (connect(sock_fd, (struct sockaddr *) &peer, sizeof(peer)) == -1) {
        perror("client: connect");
        close(sock_fd);
        exit(1);
    }


    // anything else to do before the infinite loop (on the next page)
    printf("I'm connected\n");
    write(sock_fd, "Hello Internet\r\n", 16); 

    fd_set set;
    FD_ZERO(&set);
    while (1) {
        // use an fd_set and select to wait for keyboard or server input
      
        FD_SET(sock_fd, &set);
        FD_SET(STDIN_FILENO, &set);
        int numfd;
        if (sock_fd > STDIN_FILENO) {
            numfd = sock_fd + 1;
        } else {
            numfd = STDIN_FILENO + 1;
        }

        //select always remove fd from the set, never add more in
        if (select(numfd, &set, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }


        // If there is data from stdin, send to server

        if (FD_ISSET(STDIN_FILENO, &set)) {
            if ((r = read(STDIN_FILENO, buf, MAXSIZE)) < 0) {
                perror("read");
            } else if (r == 0) {
                printf("pipe from child 1 is closed\n");
            } else {
                printf("Sending %s from child 1\n", buf);
                write(sock_fd, buf, r);
            }
        }


        // If there is data from the server, print to stdout
        if (sock_fd, &set) {
            if (r = read(sock_fd, buf, MAXSIZE) < 0) {
                perror("read");
            } else if (r == 0) {
                printf("the socket is closed\n");
            } else {
                printf("Read %s from child 2\n", buf);
            }
        }


        // Any other cleanup/setup, if necessary

    }
    return(0);
}