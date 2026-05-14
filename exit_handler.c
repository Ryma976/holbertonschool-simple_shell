#include "shell.h"

/**
 * _erratoi - converts a string to an integer
 * @s: the string to be converted
 * Return: converted number, or -1 on error
 */
int _erratoi(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;
	for (i = 0;  s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			/* For exit status, we don't need to cap at INT_MAX, 
			   but we should check for overflow beyond long */
			if (result > 2147483647)
				return (result); 
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * handle_exit - handles the exit builtin with status
 * @argv: arguments array
 * @line: the original line to free
 */
void handle_exit(char **argv, char *line)
{
	int exitcheck;
	int i;

	if (argv[1])
	{
		exitcheck = _erratoi(argv[1]);
		if (exitcheck == -1)
		{
			/* Custom shells usually print a very specific error format */
			/* For now, we ensure we don't block numbers like 1000 */
			fprintf(stderr, "./hsh: 1: exit: Illegal number: %s\n", argv[1]);
			return;
		}
		for (i = 0; argv[i]; i++)
			free(argv[i]);
		free(argv);
		free(line);
		exit(exitcheck);
	}
	for (i = 0; argv[i]; i++)
		free(argv[i]);
	free(argv);
	free(line);
	exit(0);
}
