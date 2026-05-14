#include "shell.h"

/**
 * free_argv_array - frees an argv array
 * @argv: arguments array
 *
 * Return: nothing
 */
void free_argv_array(char **argv)
{
	int i;

	if (argv == NULL)
		return;

	for (i = 0; argv[i]; i++)
		free(argv[i]);

	free(argv);
}

/**
 * replace_alias - replaces command name with alias value
 * @argv: command arguments
 *
 * Return: new argv, or old argv
 */
char **replace_alias(char **argv)
{
	char *value;
	char *new_cmd;
	char **new_argv;
	int i, len = 0;

	if (argv == NULL || argv[0] == NULL)
		return (argv);

	value = get_alias_value(argv[0]);
	if (value == NULL)
		return (argv);

	len = strlen(value) + 1;

	for (i = 1; argv[i]; i++)
		len += strlen(argv[i]) + 1;

	new_cmd = malloc(sizeof(char) * len);
	if (new_cmd == NULL)
		return (argv);

	strcpy(new_cmd, value);

	for (i = 1; argv[i]; i++)
	{
		strcat(new_cmd, " ");
		strcat(new_cmd, argv[i]);
	}

	free_argv_array(argv);

	new_argv = strtow(new_cmd, " \t\r\n\a");
	free(new_cmd);

	return (new_argv);
}
