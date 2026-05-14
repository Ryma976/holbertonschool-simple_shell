#include "shell.h"

alias_t *alias_list = NULL;

/**
 * find_alias - finds an alias by name
 * @name: alias name
 *
 * Return: alias node, or NULL
 */
alias_t *find_alias(char *name)
{
	alias_t *current;

	current = alias_list;

	while (current != NULL)
	{
		if (strcmp(current->name, name) == 0)
			return (current);

		current = current->next;
	}

	return (NULL);
}

/**
 * print_alias_node - prints one alias
 * @node: alias node
 *
 * Return: nothing
 */
void print_alias_node(alias_t *node)
{
	write(STDOUT_FILENO, node->name, strlen(node->name));
	write(STDOUT_FILENO, "='", 2);
	write(STDOUT_FILENO, node->value, strlen(node->value));
	write(STDOUT_FILENO, "'\n", 2);
}

/**
 * print_all_aliases - prints all aliases
 *
 * Return: nothing
 */
void print_all_aliases(void)
{
	alias_t *current;

	current = alias_list;

	while (current != NULL)
	{
		print_alias_node(current);
		current = current->next;
	}
}

/**
 * get_alias_value - gets alias value
 * @name: alias name
 *
 * Return: alias value, or NULL
 */
char *get_alias_value(char *name)
{
	alias_t *node;

	node = find_alias(name);
	if (node == NULL)
		return (NULL);

	return (node->value);
}

/**
 * free_aliases - frees all aliases
 *
 * Return: nothing
 */
void free_aliases(void)
{
	alias_t *current;
	alias_t *next;

	current = alias_list;

	while (current != NULL)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}

	alias_list = NULL;
}
