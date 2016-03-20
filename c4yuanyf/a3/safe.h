#ifndef SAFE_H
#define SAFE_H
#include <stdio.h>

void safe_close(int num);
void safe_dup2(int p,int q);
int safe_read(int fd, void *buf, int count);
int safe_write(int fd, void *buf, int count);
int safe_fork();
void safe_pipe(int *fd);
FILE* safe_fopen(char *path,char *mode);
void safe_fclose(FILE * file);
#endif