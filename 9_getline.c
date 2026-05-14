#include "shell.h"

/**
 * _getline - Reads input from a file descriptor
 * @lineptr: Buffer to store the input
 * @n: Size of the buffer
 * @fd: File descriptor to read from
 *
 * Return: Number of bytes read, or -1 on failure
 */
ssize_t _getline(char **lineptr, size_t *n, int fd)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t count = 0;
	char c;

	if (*lineptr == NULL || *n == 0)
	{
		*n = READ_BUF_SIZE;
		*lineptr = malloc(*n);
		if (!(*lineptr))
			return (-1);
	}

	while (1)
	{
		if (i >= len)
		{
			len = read(fd, buf, READ_BUF_SIZE);
			i = 0;
			if (len <= 0)
				return (count == 0 ? -1 : (ssize_t)count);
		}

		c = buf[i++];

		if (count >= *n - 1)
		{
			*n += READ_BUF_SIZE;
			*lineptr = realloc(*lineptr, *n);
			if (!(*lineptr))
				return (-1);
		}

		(*lineptr)[count++] = c;

		if (c == '\n')
			break;
	}

	(*lineptr)[count] = '\0';
	return (count);
}
