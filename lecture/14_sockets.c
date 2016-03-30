#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>    /* Internet domain header */

#define PORT_NUM 30000
#define MAX_BACKLOG 5

void do_stuff(int fd);

int main() {

    // Create socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("server: socket");
        exit(1);
    }

    // Bind socket to an address
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT_NUM);  
    // host to sockets
    server.sin_addr.s_addr = INADDR_ANY;
    //starting from byte 0 to byte 8
    memset(&server.sin_zero, 0, 8);  // Initialize sin_zero to 0

    //sockaddr is a different type than sockadd_in
    if (bind(sock_fd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0) {
        perror("server: bind");
        close(sock_fd);
        exit(1);
    }

    // Create queue in kernel for new connection requests
    if (listen(sock_fd, MAX_BACKLOG) < 0) {
        perror("server: listen");
        close(sock_fd);
        exit(1);
    }

    // Accept a new connection
    int client_fd = accept(sock_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("server: accept");
        close(sock_fd);
        exit(1);
    }

    printf("Got a new connection.\n");
    do_stuff(client_fd);

    close(client_fd);
    close(sock_fd);

    return 0;
}


void do_stuff(int fd) {
    char buf[128];
    int num_read = read(fd, buf, 127);
    buf[num_read] = '\0';
    printf("Server read %d bytes: %s", num_read, buf);
    write(fd, "lol good one\r\n", 14);
}
