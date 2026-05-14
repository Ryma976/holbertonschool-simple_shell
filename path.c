#include "shell.h"

/**
 * _getenv - gets an environment variable
 * @name: variable name
 * Return: pointer to value
 */
char *_getenv(const char *name)
{
	int i = 0;
	size_t len = strlen(name);

	while (environ[i])
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (environ[i] + len + 1);
		i++;
	}
	return (NULL);
}
