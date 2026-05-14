#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define READ_BUF_SIZE 1024

/* Global environment variable */
extern char **environ;

/* Function prototypes */
ssize_t _getline(char **lineptr, size_t *n, int fd);
char *find_command(char *command);
char *_getenv(const char *name);

#endif
