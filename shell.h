#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

ssize_t _getline(char **lineptr, size_t *n, FILE *stream);
char *_getenv(const char *name);
char *find_command(char *command);

#endif
