#include "shell.h"

/**
 * set_env_value - changes one environment variable
 * @name: variable name
 * @value: variable value
 *
 * Return: 0 on success, -1 on failure
 */
static int set_env_value(char *name, char *value)
{
	char *args[4];

	args[0] = "setenv";
	args[1] = name;
	args[2] = value;
	args[3] = NULL;

	return (_setenv(args));
}
/**
 * print_cd_error - prints an error for cd
 * @message: error message
 * @path: directory name
 *
 * Return: nothing
 */
static void print_cd_error(char *message, char *path)
{
	write(STDERR_FILENO, "./hsh: 1: cd: ", 15);
	write(STDERR_FILENO, message, strlen(message));

	if (path != NULL)
		write(STDERR_FILENO, path, strlen(path));

	write(STDERR_FILENO, "\n", 1);
}
/**
 * get_old_dir - gets the directory before cd
 * @old_alloc: tells if old directory must be freed
 *
 * Return: old directory, or NULL
 */
static char *get_old_dir(int *old_alloc)
{
	char *old_dir;

	*old_alloc = 0;
	old_dir = getcwd(NULL, 0);

	if (old_dir != NULL)
	{
		*old_alloc = 1;
		return (old_dir);
	}

	return (_getenv("PWD"));
}

/**
 * update_dirs - updates OLDPWD and PWD
 * @old_dir: old directory before cd
 *
 * Return: 0 on success, -1 on failure
 */
static int update_dirs(char *old_dir)
{
	char *new_dir;
	int status = 0;

	new_dir = getcwd(NULL, 0);
	if (new_dir == NULL)
		return (-1);

	if (old_dir != NULL && set_env_value("OLDPWD", old_dir) == -1)
		status = -1;

	if (set_env_value("PWD", new_dir) == -1)
		status = -1;

	free(new_dir);
	return (status);
}

/**
 * _cd - changes the current directory
 * @argv: command arguments
 *
 * Return: 0 on success, -1 on failure
 */
int _cd(char **argv)
{
	char *path, *old_dir;
	int old_alloc, print_path = 0;

	if (argv[1] != NULL && argv[2] != NULL)
	{
		print_cd_error("too many arguments", NULL);
		return (-1);
	}

	if (argv[1] == NULL)
	{
		path = _getenv("HOME");
		if (path == NULL)
			return (0);
	}
	else if (strcmp(argv[1], "-") == 0)
	{
		path = _getenv("OLDPWD");
		if (path == NULL)
		{
			print_cd_error("OLDPWD not set", NULL);
			return (-1);
		}
		print_path = 1;
	}
	else
	{
		path = argv[1];
	}

	old_dir = get_old_dir(&old_alloc);

	if (chdir(path) == -1)
	{
		print_cd_error("can't cd to ", path);
		if (old_alloc == 1)
			free(old_dir);
		return (-1);
	}

	if (update_dirs(old_dir) == -1)
	{
		if (old_alloc == 1)
			free(old_dir);
		return (-1);
	}

	if (print_path == 1)
	{
		path = _getenv("PWD");
		if (path != NULL)
		{
			write(STDOUT_FILENO, path, strlen(path));
			write(STDOUT_FILENO, "\n", 1);
		}
	}

	if (old_alloc == 1)
		free(old_dir);

	return (0);
}
