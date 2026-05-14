#include "shell.h"

/**
 * _erratoi - converts a string to an integer
 * @s: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
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
			if (result > 2147483647)
				return (-1);
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

	if (argv[1]) /* If there is an argument like exit 98 */
	{
		exitcheck = _erratoi(argv[1]);
		if (exitcheck == -1)
		{
			/* Handle error if not a valid number */
			perror("Illegal number");
			return;
		}
		/* Free everything before exiting */
		for (i = 0; argv[i]; i++)
			free(argv[i]);
		free(argv);
		free(line);
		exit(exitcheck);
	}
	/* Default exit without status */
	for (i = 0; argv[i]; i++)
		free(argv[i]);
	free(argv);
	free(line);
	exit(0);
}
