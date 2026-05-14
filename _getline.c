#include "shell.h"

/**
 * _getline - custom getline function using a static buffer
 * @lineptr: pointer to the buffer storage
 * @n: size of the buffer
 * @stream: stream to read from
 * Return: number of characters read
 */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
	static char buffer[1024];
	static size_t pos, size;
	size_t i = 0;
	char c;

	if (lineptr == NULL || n == NULL)
		return (-1);
	if (*lineptr == NULL || *n == 0)
	{
		*n = 1024;
		*lineptr = malloc(*n);
		if (*lineptr == NULL)
			return (-1);
	}
	while (1)
	{
		if (pos >= size)
		{
			size = read(fileno(stream), buffer, 1024);
			pos = 0;
			if (size <= 0)
				return (i == 0 ? -1 : (ssize_t)i);
		}
		c = buffer[pos++];
		if (i >= *n - 1)
		{
			*n += 1024;
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
