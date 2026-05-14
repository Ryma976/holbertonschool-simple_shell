#include "shell.h"

static char **env_copy;
static int env_changed;

/**
 * copy_string - makes a copy of a string
 * @str: string to copy
 *
 * Return: new string, or NULL on failure
 */
static char *copy_string(char *str)
{
	char *copy;
	int len;

	len = strlen(str) + 1;
	copy = malloc(sizeof(char) * len);
	if (copy == NULL)
		return (NULL);

	strcpy(copy, str);
	return (copy);
}

/**
 * make_env_copy - copies the environment
 *
 * Return: 0 on success, -1 on failure
 */
static int make_env_copy(void)
{
	int i, j;

	if (env_changed == 1)
		return (0);

	for (i = 0; environ[i]; i++)
		;

	env_copy = malloc(sizeof(char *) * (i + 1));
	if (env_copy == NULL)
		return (-1);

	for (j = 0; j < i; j++)
	{
		env_copy[j] = copy_string(environ[j]);
		if (env_copy[j] == NULL)
		{
			while (j > 0)
			{
				j--;
				free(env_copy[j]);
			}
			free(env_copy);
			return (-1);
		}
	}

	env_copy[i] = NULL;
	environ = env_copy;
	env_changed = 1;

	return (0);
}

/**
 * _free_env - frees the copied environment
 *
 * Return: nothing
 */
void _free_env(void)
{
	int i;

	if (env_changed == 0 || env_copy == NULL)
		return;

	for (i = 0; env_copy[i]; i++)
		free(env_copy[i]);

	free(env_copy);
	env_copy = NULL;
	env_changed = 0;
}

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
	char **new_env;
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

	if (make_env_copy() == -1)
		return (-1);

	name_len = strlen(argv[1]);
	len = strlen(argv[1]) + strlen(argv[2]) + 2;

	new_var = malloc(sizeof(char) * len);
	if (new_var == NULL)
		return (-1);

	strcpy(new_var, argv[1]);
	strcat(new_var, "=");
	strcat(new_var, argv[2]);

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], argv[1], name_len) == 0 &&
		    environ[i][name_len] == '=')
		{
			free(environ[i]);
			environ[i] = new_var;
			return (0);
		}
	}

	new_env = malloc(sizeof(char *) * (i + 2));
	if (new_env == NULL)
	{
		free(new_var);
		return (-1);
	}

	for (i = 0; environ[i]; i++)
		new_env[i] = environ[i];

	new_env[i] = new_var;
	new_env[i + 1] = NULL;

	free(env_copy);
	env_copy = new_env;
	environ = env_copy;

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

	if (make_env_copy() == -1)
		return (-1);

	len = strlen(argv[1]);

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], argv[1], len) == 0 &&
		    environ[i][len] == '=')
		{
			free(environ[i]);

			for (j = i; environ[j]; j++)
				environ[j] = environ[j + 1];

			return (0);
		}
	}

	return (0);
}
