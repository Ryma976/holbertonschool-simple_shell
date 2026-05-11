#include"shell.h"

/**
 * main - simple shell 0.2 (handling arguments)
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *argv[100];
	char *command_path;
	int i;
	pid_t child_pid;
	int status;
	int line_count = 1;
	int exit_status = 0;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}
		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		i = 0;
		argv[i] = strtok(line, " \t\r\n\a");
		while (argv[i] != NULL)
		{
			i++;
			argv[i] = strtok(NULL, " \t\r\n\a");
		}

		if (argv[0] == NULL)
			continue;

		command_path = find_command(argv[0]);
		if (command_path == NULL)
		{
			fprintf(stderr, "./hsh: %d: %s: not found\n", line_count, argv[0]);
			exit_status = 127;
			line_count++;
			continue;
		}

		child_pid = fork();
		if (child_pid == 0)
		{
			if (execve(command_path, argv, environ) == -1)
				perror("./hsh");
			free(command_path);
			exit(EXIT_FAILURE);
		}
		else
		{
			wait(&status);
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
			free(command_path);
		}
		line_count++;
	}
	free(line);
	return (exit_status);
}
