#include "shell.h"

/**
 * _env - prints the current environment
 *
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
 * _setenv - adds or changes an environment variable
 * @argv: array of arguments
 *
 * Return: 0 on success, -1 on failure
 */
int _setenv(char **argv)
{
	char *new_var;
	char **new_environ;
	int i, len, name_len;

	if (argv[1] == NULL || argv[2] == NULL || argv[3] != NULL)
	{
		write(STDERR_FILENO, "Usage: setenv VARIABLE VALUE\n", 29);
		return (-1);
	}

	if (argv[1][0] == '\0' || strchr(argv[1], '=') != NULL)
	{
		write(STDERR_FILENO, "Invalid variable name\n", 22);
		return (-1);
	}

	name_len = strlen(argv[1]);
	len = strlen(argv[1]) + strlen(argv[2]) + 2;

	new_var = malloc(sizeof(char) * len);
	if (new_var == NULL)
	{
		perror("malloc");
		return (-1);
	}

	strcpy(new_var, argv[1]);
	strcat(new_var, "=");
	strcat(new_var, argv[2]);

	for (i = 0; environ && environ[i]; i++)
	{
		if (strncmp(environ[i], argv[1], name_len) == 0 &&
		    environ[i][name_len] == '=')
		{
			environ[i] = new_var;
			return (0);
		}
	}

	new_environ = malloc(sizeof(char *) * (i + 2));
	if (new_environ == NULL)
	{
		free(new_var);
		perror("malloc");
		return (-1);
	}

	for (i = 0; environ && environ[i]; i++)
		new_environ[i] = environ[i];

	new_environ[i] = new_var;
	new_environ[i + 1] = NULL;
	environ = new_environ;

	return (0);
}

/**
 * _unsetenv - removes an environment variable
 * @argv: array of arguments
 *
 * Return: 0 on success, -1 on failure
 */
int _unsetenv(char **argv)
{
	int i, j, len;

	if (argv[1] == NULL || argv[2] != NULL)
	{
		write(STDERR_FILENO, "Usage: unsetenv VARIABLE\n", 25);
		return (-1);
	}

	if (argv[1][0] == '\0' || strchr(argv[1], '=') != NULL)
	{
		write(STDERR_FILENO, "Invalid variable name\n", 22);
		return (-1);
	}

	len = strlen(argv[1]);

	if (environ == NULL)
		return (0);

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], argv[1], len) == 0 &&
		    environ[i][len] == '=')
		{
			for (j = i; environ[j]; j++)
				environ[j] = environ[j + 1];

			return (0);
		}
	}

	return (0);
}
