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

/* Prototypes */
ssize_t _getline(char **lineptr, size_t *n, int fd);
char *_getenv(const char *name);
char *find_command(char *command);
char **strtow(char *str, char *d); /* Add this line */

#endif
