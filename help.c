#include "shell.h"

/**
 * print_help_text - prints help text
 * @text: text to print
 *
 * Return: nothing
 */
static void print_help_text(char *text)
{
	write(STDOUT_FILENO, text, strlen(text));
}

/**
 * print_builtin_help - prints help for one builtin
 * @name: builtin name
 *
 * Return: 0 if found, -1 if not found
 */
static int print_builtin_help(char *name)
{
	if (strcmp(name, "cd") == 0)
		print_help_text("cd [DIRECTORY]\nChange the current directory.\n");
	else if (strcmp(name, "exit") == 0)
		print_help_text("exit [STATUS]\nExit the shell.\n");
	else if (strcmp(name, "env") == 0)
		print_help_text("env\nPrint the environment.\n");
	else if (strcmp(name, "setenv") == 0)
		print_help_text("setenv VARIABLE VALUE\nSet an environment variable.\n");
	else if (strcmp(name, "unsetenv") == 0)
		print_help_text("unsetenv VARIABLE\nRemove an environment variable.\n");
	else if (strcmp(name, "alias") == 0)
		print_help_text("alias [name[='value'] ...]\nDisplay or set aliases.\n");
	else if (strcmp(name, "help") == 0)
		print_help_text("help [BUILTIN]\nDisplay help for a builtin.\n");
	else
		return (-1);

	return (0);
}

/**
 * _help - handles the help builtin
 * @argv: command arguments
 *
 * Return: 0 on success, -1 on failure
 */
int _help(char **argv)
{
	int i;

	if (argv[1] == NULL)
	{
		print_help_text("Simple shell builtins:\n");
		print_help_text("cd\nexit\nenv\nsetenv\nunsetenv\nalias\nhelp\n");
		print_help_text("Use help [BUILTIN] for more information.\n");
		return (0);
	}

	for (i = 1; argv[i]; i++)
	{
		if (print_builtin_help(argv[i]) == -1)
		{
			write(STDERR_FILENO, "help: no help topics match `", 28);
			write(STDERR_FILENO, argv[i], strlen(argv[i]));
			write(STDERR_FILENO, "'\n", 2);
			return (-1);
		}
	}

	return (0);
}
