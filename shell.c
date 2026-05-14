#include "shell.h"

/**
 * main - simple shell with custom _getline
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_status;
	char *argv[100];
	int i;
	pid_t child_pid;
	int status;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);read_status = _getline(&line, &len, STDIN_FILENO);
		
		if (read_status == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (line[read_status - 1] == '\n')
			line[read_status - 1] = '\0';

		i = 0;
		argv[i] = strtok(line, " \t\r\n\a");
		while (argv[i] != NULL)
		{
			i++;
			argv[i] = strtok(NULL, " \t\r\n\a");
		}

		if (argv[0] == NULL)
			continue;

		child_pid = fork();
		if (child_pid == 0)
		{
			if (execve(argv[0], argv, environ) == -1)
				perror("./hsh");
			exit(EXIT_FAILURE);
		}
		else
		{
			wait(&status);
		}
	}
	free(line);
	return (0);
}
