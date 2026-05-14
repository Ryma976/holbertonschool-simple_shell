#include "shell.h"

char *history_list[HISTORY_MAX];
int history_count;
int history_next;

/**
 * history_add - adds one command to history
 * @line: command line
 *
 * Return: nothing
 */
void history_add(char *line)
{
	char *copy;
	int i;

	if (line == NULL || line[0] == '\0')
		return;

	copy = alias_strdup(line);
	if (copy == NULL)
		return;

	if (history_count == HISTORY_MAX)
	{
		free(history_list[0]);

		for (i = 1; i < HISTORY_MAX; i++)
			history_list[i - 1] = history_list[i];

		history_count--;
	}

	history_list[history_count] = copy;
	history_count++;
	history_next = (history_next + 1) % HISTORY_MAX;
}

/**
 * _history - prints the command history
 *
 * Return: 0 on success
 */
int _history(void)
{
	int i, number, start;
	char buffer[32];

	start = history_next - history_count;

	if (start < 0)
		start += HISTORY_MAX;

	for (i = 0; i < history_count; i++)
	{
		number = (start + i) % HISTORY_MAX;
		sprintf(buffer, "%d", number);

		write(STDOUT_FILENO, buffer, strlen(buffer));
		write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, history_list[i], strlen(history_list[i]));
		write(STDOUT_FILENO, "\n", 1);
	}

	return (0);
}

/**
 * free_history - frees the history list
 *
 * Return: nothing
 */
void free_history(void)
{
	int i;

	for (i = 0; i < history_count; i++)
	{
		free(history_list[i]);
		history_list[i] = NULL;
	}

	history_count = 0;
	history_next = 0;
}
