#include "shell.h"

/**
 * handle_exit - handles exit with optional status or last command status
 * @argv: arguments
 * @line: line buffer
 * @last_status: the status of the last executed command
 */
void handle_exit(char **argv, char *line, int last_status)
{
	int exitcheck;
	int i;

	if (argv[1])
	{
		exitcheck = _erratoi(argv[1]);
		if (exitcheck == -1)
		{
			fprintf(stderr, "./hsh: 1: exit: Illegal number: %s\n", argv[1]);
			for (i = 0; argv[i]; i++)
				free(argv[i]);
			free(argv);
			free(line);
			exit(2);
		}
		for (i = 0; argv[i]; i++)
			free(argv[i]);
		free(argv);
		free(line);
		exit(exitcheck % 256);
	}
	
	/* If no argument, exit with the status of the last command */
	for (i = 0; argv[i]; i++)
		free(argv[i]);
	free(argv);
	free(line);
	exit(last_status);
}
