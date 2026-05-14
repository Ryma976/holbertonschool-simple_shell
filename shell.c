#include "shell.h"

/**
 * main - entry point for the simple shell
 * Return: last exit status
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_status;
	char **argv;
	int i, last_status = 0;

	/* Register the Ctrl+C signal handler */
	signal(SIGINT, sigint_handler);

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
			if (argv) free(argv);
			continue;
		}

		if (strcmp(argv[0], "exit") == 0)
			handle_exit(argv, line, last_status);

		execute_command(argv, line, &last_status);

		for (i = 0; argv[i]; i++) free(argv[i]);
		free(argv);
	}
	free(line);
	return (last_status);
}
