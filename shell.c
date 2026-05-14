#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define READ_BUF_SIZE 1024

extern char **environ;

/**
 * _getline - Custom getline function using a static buffer
 * @lineptr: Pointer to the buffer that stores the line
 * @n: Size of the buffer
 * @fd: File descriptor to read from
 *
 * Return: Number of bytes read or -1 on failure
 */
ssize_t _getline(char **lineptr, size_t *n, int fd)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t count = 0;
	char c;

	if (*lineptr == NULL || *n == 0)
	{
		*n = READ_BUF_SIZE;
		*lineptr = malloc(*n);
		if (!(*lineptr))
			return (-1);
	}

	while (1)
	{
		if (i >= len)
		{
			len = read(fd, buf, READ_BUF_SIZE);
			i = 0;
			if (len <= 0)
				return (count == 0 ? -1 : (ssize_t)count);
		}
		c = buf[i++];
		if (count >= *n - 1)
		{
			*n += READ_BUF_SIZE;
			*lineptr = realloc(*lineptr, *n);
			if (!(*lineptr))
				return (-1);
		}
		(*lineptr)[count++] = c;
		if (c == '\n')
			break;
	}
	(*lineptr)[count] = '\0';
	return (count);
}

/**
 * find_path - Locates a command in the PATH environment variable
 * @command: The command to find
 *
 * Return: Full path of the command or NULL if not found
 */
char *find_path(char *command)
{
	char *path = NULL, *path_copy, *token, *full_path;
	struct stat st;
	int i = 0;

	if (strchr(command, '/') != NULL)
		return (access(command, X_OK) == 0 ? strdup(command) : NULL);

	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path = environ[i] + 5;
			break;
		}
		i++;
	}
	if (!path)
		return (NULL);

	path_copy = strdup(path);
	token = strtok(path_copy, ":");
	while (token)
	{
		full_path = malloc(strlen(token) + strlen(command) + 2);
		sprintf(full_path, "%s/%s", token, command);
		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

/**
 * main - Main loop for the simple shell
 *
 * Return: Always 0
 */
int main(void)
{
	char *line = NULL, *argv[64], *path;
	size_t n = 0;
	ssize_t nread;
	int i, status;
	pid_t child_pid;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);

		nread = _getline(&line, &n, STDIN_FILENO);
		if (nread == -1)
			break;

		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		i = 0;
		argv[i] = strtok(line, " \t\n");
		if (!argv[i])
			continue;

		if (strcmp(argv[0], "exit") == 0)
			break;

		path = find_path(argv[0]);
		if (path)
		{
			child_pid = fork();
			if (child_pid == 0)
			{
				while (argv[i])
					argv[++i] = strtok(NULL, " \t\n");
				execve(path, argv, environ);
				perror("Error");
				exit(1);
			}
			wait(&status);
			free(path);
		}
		else
			perror("./hsh");
	}
	free(line);
	return (0);
}
