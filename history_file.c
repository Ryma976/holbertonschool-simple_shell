#include "shell.h"

/**
 * get_history_path - builds the history file path
 *
 * Return: history file path, or NULL on failure
 */
char *get_history_path(void)
{
	char *home;
	char *path;
	int len;

	home = _getenv("HOME");
	if (home == NULL)
		return (NULL);

	len = strlen(home) + strlen("/.simple_shell_history") + 1;

	path = malloc(sizeof(char) * len);
	if (path == NULL)
		return (NULL);

	strcpy(path, home);
	strcat(path, "/.simple_shell_history");

	return (path);
}

/**
 * load_history - reads history from file
 *
 * Return: nothing
 */
void load_history(void)
{
	char *path;
	char line[READ_BUF_SIZE];
	char c;
	int fd, i;
	ssize_t r;

	path = get_history_path();
	if (path == NULL)
		return;

	fd = open(path, O_RDONLY);
	free(path);

	if (fd == -1)
		return;

	i = 0;

	while ((r = read(fd, &c, 1)) > 0)
	{
		if (c == '\n')
		{
			line[i] = '\0';
			history_add(line);
			i = 0;
		}
		else if (i < READ_BUF_SIZE - 1)
		{
			line[i] = c;
			i++;
		}
	}

	if (i > 0)
	{
		line[i] = '\0';
		history_add(line);
	}

	close(fd);
}

/**
 * save_history - writes history to file
 *
 * Return: nothing
 */
void save_history(void)
{
	char *path;
	int fd, i;

	path = get_history_path();
	if (path == NULL)
		return;

	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	free(path);

	if (fd == -1)
		return;

	for (i = 0; i < history_count; i++)
	{
		write(fd, history_list[i], strlen(history_list[i]));
		write(fd, "\n", 1);
	}

	close(fd);
}
