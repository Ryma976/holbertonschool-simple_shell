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

/**
 * struct alias_s - alias node
 * @name: alias name
 * @value: alias value
 * @next: next node
 */
typedef struct alias_s
{
	char *name;
	char *value;
	struct alias_s *next;
} alias_t;

extern alias_t *alias_list;
int _alias(char *cmd);
alias_t *find_alias(char *name);
void print_alias_node(alias_t *node);
void print_all_aliases(void);
char *get_alias_value(char *name);
void free_aliases(void);
char *alias_strdup(char *str);
int set_alias_value(char *name, char *value);
int set_alias_from_arg(char *arg);
char *next_alias_arg(char **line);
void free_argv_array(char **argv);
char **replace_alias(char **argv);
char *replace_variables(char *line, int last_status);
extern char **environ;
ssize_t _getline(char **lineptr, size_t *n, int fd);
char **strtow(char *str, char *d);
void handle_exit(char **argv, char *line, int last_status);
void execute_command(char **argv, char *line, int *last_status);
void run_command(char *cmd, char *line, int *last_status);
void handle_separator(char *line, int *last_status);
int _erratoi(char *s);
void sigint_handler(int sig);
int _env(void);
int _setenv(char **argv);
int _unsetenv(char **argv);
void _free_env(void);
char *_getenv(const char *name);
int _cd(char **argv);
#endif
