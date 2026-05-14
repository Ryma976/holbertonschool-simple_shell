#include "shell.h"

/**
 * main - shell entry point
 * @argc: argument count
 * @argv: argument values
 *
 * Return: last exit status
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_status;
	int last_status = 0;
	int input_fd = STDIN_FILENO;

	signal(SIGINT, sigint_handler);
	load_history();

	if (argc > 1)
	{
		input_fd = open(argv[1], O_RDONLY);
		if (input_fd == -1)
		{
			write(STDERR_FILENO, "./hsh: 0: Can't open ", 21);
			write(STDERR_FILENO, argv[1], strlen(argv[1]));
			write(STDERR_FILENO, "\n", 1);
			return (127);
		}
	}

	while (1)
	{
		if (input_fd == STDIN_FILENO && isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		read_status = _getline(&line, &len, input_fd);
		if (read_status == -1)
		{
			if (input_fd == STDIN_FILENO && isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (line[read_status - 1] == '\n')
			line[read_status - 1] = '\0';

		history_add(line);
		remove_comments(line);
		handle_separator(line, &last_status);
	}

	if (input_fd != STDIN_FILENO)
		close(input_fd);

	free(line);
	save_history();
	free_history();
	free_aliases();
	_free_env();

	return (last_status);
}

/**
 * remove_comments - removes comments from input line
 * @line: input line
 *
 * Return: nothing
 */
void remove_comments(char *line)
{
	int i;

	if (line == NULL)
		return;

	for (i = 0; line[i] != '\0'; i++)
	{
		if (line[i] == '#')
		{
			if (i == 0 || line[i - 1] == ' ' || line[i - 1] == '\t' ||
			    line[i - 1] == ';' || line[i - 1] == '&' ||
			    line[i - 1] == '|')
			{
				line[i] = '\0';
				return;
			}
		}
	}
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

	if (strcmp(argv[0], "exit") == 0)
		handle_exit(argv, line, *last_status);
	else if (strcmp(argv[0], "env") == 0)
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
	else if (strcmp(argv[0], "help") == 0)
	{
		if (_help(argv) == -1)
			*last_status = 1;
		else
			*last_status = 0;
	}
	else if (strcmp(argv[0], "history") == 0)
	{
		_history();
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
	char *cmd_path;

	cmd_path = find_command_path(argv[0]);
	if (cmd_path == NULL)
	{
		write(STDERR_FILENO, "./hsh: 1: ", 10);
		write(STDERR_FILENO, argv[0], strlen(argv[0]));
		write(STDERR_FILENO, ": not found\n", 12);
		*last_status = 127;
		return;
	}

	child_pid = fork();

	if (child_pid == 0)
	{
		if (execve(cmd_path, argv, environ) == -1)
		{
			perror("./hsh");

			for (i = 0; argv[i]; i++)
				free(argv[i]);

			free(argv);
			free(line);
			free(cmd_path);
			free_history();
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

	free(cmd_path);
}
