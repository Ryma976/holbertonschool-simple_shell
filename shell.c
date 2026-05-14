#include "shell.h"

/**
 * main - simple shell with built-in exit handling
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_status;
	char **argv;
	int i;

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

		/* CRITICAL: Built-in exit check MUST be first */
		if (strcmp(argv[0], "exit") == 0)
		{
			handle_exit(argv, line);
			/* If handle_exit returns, it's due to an error; clean up */
			for (i = 0; argv[i]; i++)
				free(argv[i]);
			free(argv);
			continue;
		}

		/* Only if not exit, proceed to fork and execve */
		execute_command(argv, line);

		/* Cleanup argv after execution */
		for (i = 0; argv[i]; i++)
			free(argv[i]);
		free(argv);
	}
	free(line);
	return (0);
}

/**
 * execute_command - forks and executes a command
 * @argv: argument array
 * @line: original line buffer (for cleanup in child)
 */
void execute_command(char **argv, char *line)
{
	pid_t child_pid;
	int status, i;

	child_pid = fork();
	if (child_pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			perror("./hsh");
			for (i = 0; argv[i]; i++)
				free(argv[i]);
			free(argv);
			free(line);
			exit(127);
		}
	}
	else
	{
		wait(&status);
	}
}
