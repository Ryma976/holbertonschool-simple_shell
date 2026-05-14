#include "shell.h"

/**
 * main - shell entry point
 * Return: last exit status
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_status;
	char **argv;
	int i, last_status = 0;

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

		argv = strtow(line, " \t\r\n\a");
		if (argv == NULL || argv[0] == NULL)
		{
			if (argv)
				free(argv);
			continue;
		}

		if (strcmp(argv[0], "exit") == 0)
			handle_exit(argv, line, last_status);
		else if (strcmp(argv[0], "env") == 0)
			_env();
		else if (strcmp(argv[0], "setenv") == 0)
			_setenv(argv);
		else if (strcmp(argv[0], "unsetenv") == 0)
			_unsetenv(argv);
		else if (strcmp(argv[0], "cd") == 0)
			_cd(argv);
		else
			execute_command(argv, line, &last_status);

		for (i = 0; argv[i]; i++)
			free(argv[i]);
		free(argv);
	}
	free(line);
	_free_env();
	return (last_status);
}

/**
 * execute_command - forks and runs command
 * @argv: args
 * @line: buffer
 * @last_status: status pointer
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
