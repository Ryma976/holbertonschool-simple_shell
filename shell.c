#include "shell.h"

/**
 * main - shell entry point
 *
 * Return: last exit status
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_status;
	int last_status = 0;

	signal(SIGINT, sigint_handler);

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		read_status = _getline(&line, &len, STDIN_FILENO);
		if (read_status == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (line[read_status - 1] == '\n')
			line[read_status - 1] = '\0';

		handle_separator(line, &last_status);
	}

	free(line);
	free_aliases();
	_free_env();
	return (last_status);
}

/**
 * run_command - runs one command
 * @cmd: command text
 * @line: full input line
 * @last_status: last status value
 *
 * Return: nothing
 */
void run_command(char *cmd, char *line, int *last_status)
{
	char **argv;
	char *new_cmd;
	int i;

	argv = strtow(cmd, " \t\r\n\a");
	if (argv == NULL || argv[0] == NULL)
	{
		if (argv)
			free(argv);
		return;
	}

	if (strcmp(argv[0], "exit") == 0)
		handle_exit(argv, line, *last_status);

	if (strcmp(argv[0], "alias") == 0)
	{
		if (_alias(cmd) == -1)
			*last_status = 1;
		else
			*last_status = 0;

		free_argv_array(argv);
		return;
	}

	for (i = 0; argv[i]; i++)
		free(argv[i]);
	free(argv);

	new_cmd = replace_variables(cmd, *last_status);
	if (new_cmd == NULL)
		return;

	argv = strtow(new_cmd, " \t\r\n\a");
	free(new_cmd);

	if (argv == NULL || argv[0] == NULL)
	{
		if (argv)
			free(argv);
		return;
	}

	argv = replace_alias(argv);
	if (argv == NULL || argv[0] == NULL)
		return;

	if (strcmp(argv[0], "env") == 0)
	{
		_env();
		*last_status = 0;
	}
	else if (strcmp(argv[0], "setenv") == 0)
	{
		if (_setenv(argv) == -1)
			*last_status = 1;
		else
			*last_status = 0;
	}
	else if (strcmp(argv[0], "unsetenv") == 0)
	{
		if (_unsetenv(argv) == -1)
			*last_status = 1;
		else
			*last_status = 0;
	}
	else if (strcmp(argv[0], "cd") == 0)
	{
		if (_cd(argv) == -1)
			*last_status = 1;
		else
			*last_status = 0;
	}
	else
		execute_command(argv, line, last_status);

	for (i = 0; argv[i]; i++)
		free(argv[i]);

	free(argv);
}

/**
 * handle_separator - handles ;, && and || operators
 * @line: input line
 * @last_status: last status value
 *
 * Return: nothing
 */
void handle_separator(char *line, int *last_status)
{
	char *cmd;
	int i, op, run_next = 1;

	cmd = line;

	for (i = 0; ; i++)
	{
		op = 0;

		if (line[i] == '&' && line[i + 1] == '&')
		{
			op = 1;
			line[i] = '\0';
			i++;
		}
		else if (line[i] == '|' && line[i + 1] == '|')
		{
			op = 2;
			line[i] = '\0';
			i++;
		}
		else if (line[i] == ';')
		{
			op = 3;
			line[i] = '\0';
		}
		else if (line[i] == '\0')
		{
			op = 4;
		}

		if (op != 0)
		{
			if (run_next == 1)
				run_command(cmd, line, last_status);

			if (op == 4)
				break;

			if (op == 1)
				run_next = (*last_status == 0);
			else if (op == 2)
				run_next = (*last_status != 0);
			else
				run_next = 1;

			cmd = line + i + 1;
		}
	}
}

/**
 * execute_command - forks and runs command
 * @argv: args
 * @line: buffer
 * @last_status: status pointer
 *
 * Return: nothing
 */
void execute_command(char **argv, char *line, int *last_status)
{
	pid_t child_pid;
	int status, i;

	child_pid = fork();

	if (child_pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			perror("./hsh");

			for (i = 0; argv[i]; i++)
				free(argv[i]);

			free(argv);
			free(line);
			free_aliases();
			_free_env();
			_exit(127);
		}
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status))
			*last_status = WEXITSTATUS(status);
	}
}
