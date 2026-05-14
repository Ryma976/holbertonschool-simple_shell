#include "shell.h"

/**
 * _erratoi - string to int
 * @s: string
 * Return: int or -1
 */
int _erratoi(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+') s++;
	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > 2147483647) return (-1);
		}
		else return (-1);
	}
	return ((int)result);
}

/**
 * handle_exit - handles exit logic
 * @argv: args
 * @line: buffer
 * @last_status: status
 */
void handle_exit(char **argv, char *line, int last_status)
{
	int exit_val, i;

	if (argv[1])
	{
		exit_val = _erratoi(argv[1]);
		if (exit_val == -1)
		{
			write(STDERR_FILENO, "./hsh: 1: exit: Illegal number: ", 32);
			write(STDERR_FILENO, argv[1], strlen(argv[1]));
			write(STDERR_FILENO, "\n", 1);
			for (i = 0; argv[i]; i++) free(argv[i]);
			free(argv);
			free(line);
			exit(2);
		}
		for (i = 0; argv[i]; i++) free(argv[i]);
		free(argv);
		free(line);
		exit(exit_val % 256);
	}
	for (i = 0; argv[i]; i++) free(argv[i]);
	free(argv);
	free(line);
	exit(last_status);
}
