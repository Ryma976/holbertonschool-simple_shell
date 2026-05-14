#include "shell.h"

/**
 * _getenv - gets an environment variable
 * @name: variable name
 *
 * Return: pointer to value, or NULL
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

/**
 * build_path - builds full path for a command
 * @dir: directory from PATH
 * @cmd: command name
 *
 * Return: full path, or NULL on failure
 */
char *build_path(char *dir, char *cmd)
{
	char *full_path;
	int len;

	len = strlen(dir) + strlen(cmd) + 2;

	full_path = malloc(sizeof(char) * len);
	if (full_path == NULL)
		return (NULL);

	strcpy(full_path, dir);
	strcat(full_path, "/");
	strcat(full_path, cmd);

	return (full_path);
}

/**
 * find_command_path - finds command in PATH
 * @cmd: command name
 *
 * Return: full path, or NULL if not found
 */
char *find_command_path(char *cmd)
{
	char *path, *path_copy, *dir, *full_path;

	if (cmd == NULL)
		return (NULL);

	if (strchr(cmd, '/') != NULL)
		return (alias_strdup(cmd));

	path = _getenv("PATH");
	if (path == NULL)
		return (NULL);

	path_copy = alias_strdup(path);
	if (path_copy == NULL)
		return (NULL);

	dir = strtok(path_copy, ":");

	while (dir != NULL)
	{
		full_path = build_path(dir, cmd);
		if (full_path == NULL)
		{
			free(path_copy);
			return (NULL);
		}

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}
