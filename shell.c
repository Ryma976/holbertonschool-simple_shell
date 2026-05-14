#include "shell.h"

/**
 * main - simple shell tracking last exit status
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_status;
	char **argv;
	int i, last_status = 0;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		read_status = _getline(&line, &len, STDIN_FILENO);
		if (read_status == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (line[read_status - 1] == '\n')
			line[read_status - 1] = '\0';

		argv = strtow(line, " \t\r\n\a");
		if (argv == NULL || argv[0] == NULL)
		{
			if (argv)
				free(argv);
			continue;
		}

		if (strcmp(argv[0], "exit") == 0)
		{
			handle_exit(argv, line, last_status);
			for (i = 0; argv[i]; i++)
				free(argv[i]);
			free(argv);
			continue;
		}

		execute_command(argv, line, &last_status);

		for (i = 0; argv[i]; i++)
			free(argv[i]);
		free(argv);
	}
	free(line);
	return (last_status);
}

/**
 * execute_command - forks and updates last_status
 * @argv: arguments
 * @line: line buffer
 * @last_status: pointer to status tracker
 */
void execute_command(char **argv, char *line, int *last_status)
{
	pid_t child_pid;
	int status, i;

	child_pid = fork();
	if (child_pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			/* perror is usually not required for specific path errors, 
			   but check your project requirements for the exact string */
			for (i = 0; argv[i]; i++)
				free(argv[i]);
			free(argv);
			free(line);
			_exit(127);
		}
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status))
			*last_status = WEXITSTATUS(status);
	}
}
