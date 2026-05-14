#include "shell.h"

/**
 * _getline - Reads input from a file descriptor into a buffer.
 * @lineptr: Address of a pointer to a buffer.
 * @n: Size of lineptr.
 * @fd: File descriptor to read from.
 *
 * Return: Number of characters read.
 */
ssize_t _getline(char **lineptr, size_t *n, int fd)
{
	static char buffer[READ_BUF_SIZE];
	static size_t pos, size;
	size_t i = 0;
	char c;

	if (lineptr == NULL || n == NULL)
		return (-1);

	if (*lineptr == NULL || *n == 0)
	{
		*n = READ_BUF_SIZE;
		*lineptr = malloc(*n);
		if (*lineptr == NULL)
			return (-1);
	}

	while (1)
	{
		if (pos >= size)
		{
			size = read(fd, buffer, READ_BUF_SIZE);
			pos = 0;
			if (size <= 0)
				return (i == 0 ? -1 : (ssize_t)i);
		}

		c = buffer[pos++];
		if (i >= *n - 1)
		{
			*n += READ_BUF_SIZE;
			*lineptr = realloc(*lineptr, *n);
			if (*lineptr == NULL)
				return (-1);
		}

		(*lineptr)[i++] = c;
		if (c == '\n')
			break;
	}
	(*lineptr)[i] = '\0';
	return (i);
}
