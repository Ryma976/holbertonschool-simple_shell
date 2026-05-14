#include "shell.h"

/**
 * main - simple shell using custom strtow
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_status;
	char **argv;
	pid_t child_pid;
	int status, i;

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

		/* Use the new custom tokenizer */
		argv = strtow(line, " \t\r\n\a");
		if (argv == NULL || argv[0] == NULL)
		{
			if (argv)
				free(argv);
			continue;
		}

		child_pid = fork();
		if (child_pid == 0)
		{
			if (execve(argv[0], argv, environ) == -1)
				perror("./hsh");
			
			/* Free memory before exiting child */
			for (i = 0; argv[i]; i++)
				free(argv[i]);
			free(argv);
			free(line);
			exit(EXIT_FAILURE);
		}
		else
		{
			wait(&status);
			/* Free argv in parent after command finishes */
			for (i = 0; argv[i]; i++)
				free(argv[i]);
			free(argv);
		}
	}
	free(line);
	return (0);
}
