#include "shell.h"

/**
 * _getenv - Gets an environment variable
 * @name: Variable name
 *
 * Return: Variable value or NULL
 */
char *_getenv(char *name)
{
	int i = 0;
	size_t len = strlen(name);

	while (environ[i] != NULL)
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (environ[i] + len + 1);
		i++;
	}

	return (NULL);
}
