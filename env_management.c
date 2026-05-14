#include "shell.h"

/**
 * _env - prints the current environment
 * Return: 0 on success
 */
int _env(void)
{
	int i;

	if (environ == NULL)
		return (0);

	for (i = 0; environ[i]; i++)
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
	}
	return (0);
}

/**
 * _setenv - Initialize or modify an environment variable
 * @argv: array of arguments
 * Return: 0 on success, -1 on failure
 */
int _setenv(char **argv)
{
	char *new_var;
	int i, len;

	if (!argv[1] || !argv[2])
		return (-1);

	len = strlen(argv[1]) + strlen(argv[2]) + 2;
	new_var = malloc(len);
	if (!new_var)
		return (-1);

	strcpy(new_var, argv[1]);
	strcat(new_var, "=");
	strcat(new_var, argv[2]);

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], argv[1], strlen(argv[1])) == 0 &&
		    environ[i][strlen(argv[1])] == '=')
		{
			/* 
			 * We overwrite the pointer. To satisfy Valgrind, 
			 * some environments require us to not free the old one 
			 * because it might not be malloced.
			 */
			environ[i] = new_var;
			return (0);
		}
	}
	
	environ[i] = new_var;
	environ[i + 1] = NULL;
	return (0);
}

/**
 * _unsetenv - Remove an environment variable
 * @argv: array of arguments
 * Return: 0 on success, -1 on failure
 */
int _unsetenv(char **argv)
{
	int i, j, len;

	if (!argv[1])
		return (-1);

	len = strlen(argv[1]);
	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], argv[1], len) == 0 && environ[i][len] == '=')
		{
			for (j = i; environ[j]; j++)
				environ[j] = environ[j + 1];
			return (0);
		}
	}
	return (0);
}
