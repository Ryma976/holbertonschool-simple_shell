#include "shell.h"

/**
 * alias_strdup - copies a string
 * @str: string to copy
 *
 * Return: new string, or NULL
 */
char *alias_strdup(char *str)
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
 * set_alias_value - adds or updates an alias
 * @name: alias name
 * @value: alias value
 *
 * Return: 0 on success, -1 on failure
 */
int set_alias_value(char *name, char *value)
{
	alias_t *node;
	alias_t *new_node;
	char *new_value;

	node = find_alias(name);

	if (node != NULL)
	{
		new_value = alias_strdup(value);
		if (new_value == NULL)
			return (-1);

		free(node->value);
		node->value = new_value;
		return (0);
	}

	new_node = malloc(sizeof(alias_t));
	if (new_node == NULL)
		return (-1);

	new_node->name = alias_strdup(name);
	new_node->value = alias_strdup(value);

	if (new_node->name == NULL || new_node->value == NULL)
	{
		free(new_node->name);
		free(new_node->value);
		free(new_node);
		return (-1);
	}

	new_node->next = alias_list;
	alias_list = new_node;

	return (0);
}

/**
 * set_alias_from_arg - sets alias from name=value
 * @arg: alias argument
 *
 * Return: 0 on success, -1 on failure
 */
int set_alias_from_arg(char *arg)
{
	char *equal;
	char *name;
	char *value;
	int len;

	equal = strchr(arg, '=');
	if (equal == NULL)
		return (-1);

	*equal = '\0';
	name = arg;
	value = equal + 1;

	if (name[0] == '\0')
		return (-1);

	len = strlen(value);

	if (len >= 2 && value[0] == '\'' && value[len - 1] == '\'')
	{
		value[len - 1] = '\0';
		value++;
	}

	return (set_alias_value(name, value));
}

/**
 * next_alias_arg - gets the next alias argument
 * @line: command line pointer
 *
 * Return: next argument, or NULL
 */
char *next_alias_arg(char **line)
{
	char *start;
	int in_quote = 0;

	while (**line == ' ' || **line == '\t')
		(*line)++;

	if (**line == '\0')
		return (NULL);

	start = *line;

	while (**line != '\0')
	{
		if (**line == '\'')
			in_quote = !in_quote;

		if (in_quote == 0 && (**line == ' ' || **line == '\t'))
			break;

		(*line)++;
	}

	if (**line != '\0')
	{
		**line = '\0';
		(*line)++;
	}

	return (start);
}

/**
 * _alias - handles the alias builtin
 * @cmd: full alias command
 *
 * Return: 0 on success, -1 on failure
 */
int _alias(char *cmd)
{
	char *arg;
	char *ptr;
	alias_t *node;
	int status = 0;

	ptr = cmd + 5;

	arg = next_alias_arg(&ptr);
	if (arg == NULL)
	{
		print_all_aliases();
		return (0);
	}

	while (arg != NULL)
	{
		if (strchr(arg, '=') != NULL)
		{
			if (set_alias_from_arg(arg) == -1)
				status = -1;
		}
		else
		{
			node = find_alias(arg);
			if (node != NULL)
				print_alias_node(node);
		}

		arg = next_alias_arg(&ptr);
	}

	return (status);
}
