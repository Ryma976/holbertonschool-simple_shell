#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define READ_BUF_SIZE 1024

extern char **environ;

/* Prototypes updated to include last_status */
ssize_t _getline(char **lineptr, size_t *n, int fd);
char **strtow(char *str, char *d);
void handle_exit(char **argv, char *line, int last_status);
void execute_command(char **argv, char *line, int *last_status);
int _erratoi(char *s);

#endif
