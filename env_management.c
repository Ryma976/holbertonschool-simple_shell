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
 * @argv: array of arguments (argv[1]=NAME, argv[2]=VALUE)
 * Return: 0 on success, -1 on failure
 */
int _setenv(char **argv)
{
	char *new_var;
	char **new_environ;
	int i, len, count;

	if (!argv[1] || !argv[2])
	{
		write(STDERR_FILENO, "setenv: Usage: setenv VARIABLE VALUE\n", 37);
		return (-1);
	}
	len = strlen(argv[1]) + strlen(argv[2]) + 2;
	new_var = malloc(len);
	if (!new_var)
	{
		write(STDERR_FILENO, "setenv: malloc failed\n", 22);
		return (-1);
	}
	strcpy(new_var, argv[1]);
	strcat(new_var, "=");
	strcat(new_var, argv[2]);

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], argv[1], strlen(argv[1])) == 0 &&
		    environ[i][strlen(argv[1])] == '=')
		{
			environ[i] = new_var;
			return (0);
		}
	}

	count = i;
	new_environ = malloc((count + 2) * sizeof(char *));
	if (!new_environ)
	{
		free(new_var);
		write(STDERR_FILENO, "setenv: malloc failed\n", 22);
		return (-1);
	}
	for (i = 0; i < count; i++)
		new_environ[i] = environ[i];
	new_environ[count] = new_var;
	new_environ[count + 1] = NULL;
	environ = new_environ;
	return (0);
}

/**
 * _unsetenv - Remove an environment variable
 * @argv: array of arguments (argv[1]=NAME)
 * Return: 0 on success, -1 on failure
 */
int _unsetenv(char **argv)
{
	int i, j, len;

	if (!argv[1])
	{
		write(STDERR_FILENO, "unsetenv: Usage: unsetenv VARIABLE\n", 35);
		return (-1);
	}
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
