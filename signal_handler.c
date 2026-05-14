#include "shell.h"

/**
 * sigint_handler - handles SIGINT (Ctrl+C) to prevent shell from exiting
 * @sig: the signal number
 */
void sigint_handler(int sig)
{
	(void)sig; /* Unused parameter */
	write(STDOUT_FILENO, "\n($) ", 5);
}
