#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

#define READ_BUF_SIZE 1024

ssize_t _getline(char **lineptr, size_t *n, int fd);

#endif
