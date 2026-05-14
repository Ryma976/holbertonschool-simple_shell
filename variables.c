#include "shell.h"

/**
 * is_var_char - checks if a character can be in a variable name
 * @c: character to check
 *
 * Return: 1 if true, 0 if false
 */
static int is_var_char(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);

	if (c >= '0' && c <= '9')
		return (1);

	if (c == '_')
		return (1);

	return (0);
}

/**
 * make_number - changes a number to a string
 * @num: number to change
 *
 * Return: new string, or NULL on failure
 */
static char *make_number(int num)
{
	char buffer[32];

	sprintf(buffer, "%d", num);
	return (alias_strdup(buffer));
}

/**
 * get_env_part - gets the value of an environment variable
 * @line: command line
 * @index: current index
 *
 * Return: variable value, or empty string
 */
static char *get_env_part(char *line, int *index)
{
	char *name, *value, *copy;
	int start, len, i;

	start = *index + 1;
	len = 0;

	while (is_var_char(line[start + len]))
		len++;

	if (len == 0)
	{
		(*index)++;
		return (alias_strdup("$"));
	}

	name = malloc(sizeof(char) * (len + 1));
	if (name == NULL)
		return (NULL);

	for (i = 0; i < len; i++)
		name[i] = line[start + i];

	name[i] = '\0';

	value = _getenv(name);
	free(name);

	if (value == NULL)
		copy = alias_strdup("");
	else
		copy = alias_strdup(value);

	*index = start + len;

	return (copy);
}

/**
 * get_variable_value - gets the value after $
 * @line: command line
 * @index: current index
 * @last_status: last command status
 *
 * Return: new value, or NULL on failure
 */
static char *get_variable_value(char *line, int *index, int last_status)
{
	char next;

	next = line[*index + 1];

	if (next == '?')
	{
		*index += 2;
		return (make_number(last_status));
	}

	if (next == '$')
	{
		*index += 2;
		return (make_number((int)getpid()));
	}

	return (get_env_part(line, index));
}

/**
 * replace_variables - replaces variables in a command line
 * @line: command line
 * @last_status: last command status
 *
 * Return: new line, or NULL on failure
 */
char *replace_variables(char *line, int last_status)
{
	char *new_line, *value;
	int i, j, len, k;

	len = 0;

	for (i = 0; line[i]; )
	{
		if (line[i] == '$')
		{
			value = get_variable_value(line, &i, last_status);
			if (value == NULL)
				return (NULL);
			len += strlen(value);
			free(value);
		}
		else
		{
			len++;
			i++;
		}
	}

	new_line = malloc(sizeof(char) * (len + 1));
	if (new_line == NULL)
		return (NULL);

	for (i = 0, j = 0; line[i]; )
	{
		if (line[i] == '$')
		{
			value = get_variable_value(line, &i, last_status);
			if (value == NULL)
			{
				free(new_line);
				return (NULL);
			}

			for (k = 0; value[k]; k++)
				new_line[j++] = value[k];

			free(value);
		}
		else
		{
			new_line[j++] = line[i++];
		}
	}

	new_line[j] = '\0';
	return (new_line);
}
