#include "shell.h"

/**
 * _setenv - Initialize a new environment variable, or modify an existing one
 * @argv: array of arguments (setenv VARIABLE VALUE)
 * Return: 0 on success, -1 on failure
 */
int _setenv(char **argv)
{
	char *new_var;
	int i, len;

	if (!argv[1] || !argv[2])
	{
		fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		return (-1);
	}

	/* Create string format "VARIABLE=VALUE" */
	len = strlen(argv[1]) + strlen(argv[2]) + 2;
	new_var = malloc(len);
	if (!new_var) return (-1);

	strcpy(new_var, argv[1]);
	strcat(new_var, "=");
	strcat(new_var, argv[2]);

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], argv[1], strlen(argv[1])) == 0 &&
		    environ[i][strlen(argv[1])] == '=')
		{
			environ[i] = new_var; /* Update existing */
			return (0);
		}
	}
	environ[i] = new_var; /* Add new */
	environ[i + 1] = NULL;
	return (0);
}

/**
 * _unsetenv - Remove an environment variable
 * @argv: array of arguments (unsetenv VARIABLE)
 * Return: 0 on success, -1 on failure
 */
int _unsetenv(char **argv)
{
	int i, j;
	int len;

	if (!argv[1])
	{
		fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		return (-1);
	}

	len = strlen(argv[1]);
	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], argv[1], len) == 0 && environ[i][len] == '=')
		{
			for (j = i; environ[j]; j++)
			{
				environ[j] = environ[j + 1];
			}
			return (0);
		}
	}
	return (0);
}
