#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define READ_BUF_SIZE 1024

extern char **environ;

ssize_t _getline(char **lineptr, size_t *n, int fd);
char **strtow(char *str, char *d);
void handle_exit(char **argv, char *line, int last_status);
void execute_command(char **argv, char *line, int *last_status);
int _erratoi(char *s);
void sigint_handler(int sig);
int _env(void);
int _setenv(char **argv);
int _unsetenv(char **argv);
void _free_env(void);
char *_getenv(const char *name);
int _cd(char **argv);
#endif
