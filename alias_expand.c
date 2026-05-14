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
 * make_alias_command - builds a new command from alias value
 * @value: alias value
 * @argv: old arguments
 *
 * Return: new command string, or NULL on failure
 */
static char *make_alias_command(char *value, char **argv)
{
	char *new_cmd;
	int i, len;

	len = strlen(value) + 1;

	for (i = 1; argv[i]; i++)
		len += strlen(argv[i]) + 1;

	new_cmd = malloc(sizeof(char) * len);
	if (new_cmd == NULL)
		return (NULL);

	strcpy(new_cmd, value);

	for (i = 1; argv[i]; i++)
	{
		strcat(new_cmd, " ");
		strcat(new_cmd, argv[i]);
	}

	return (new_cmd);
}

/**
 * replace_alias - replaces alias names with their values
 * @argv: command arguments
 *
 * Return: new argv, or old argv
 */
char **replace_alias(char **argv)
{
	char *value;
	char *new_cmd;
	char **new_argv;
	int count = 0;

	if (argv == NULL || argv[0] == NULL)
		return (argv);

	value = get_alias_value(argv[0]);

	while (value != NULL && count < 20)
	{
		new_cmd = make_alias_command(value, argv);
		if (new_cmd == NULL)
			return (argv);

		free_argv_array(argv);

		new_argv = strtow(new_cmd, " \t\r\n\a");
		free(new_cmd);

		if (new_argv == NULL || new_argv[0] == NULL)
			return (new_argv);

		argv = new_argv;
		value = get_alias_value(argv[0]);
		count++;
	}

	return (argv);
}
