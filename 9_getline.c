#include "shell.h"

/**
 * _getline - custom getline function without using realloc
 * @lineptr: buffer storage
 * @n: size
 * @fd: file descriptor
 * Return: bytes read
 */
ssize_t _getline(char **lineptr, size_t *n, int fd)
{
	static char buf[READ_BUF_SIZE];
	static size_t pos, size;
	size_t i = 0;
	char c, *new_ptr;

	if (lineptr == NULL || n == NULL)
		return (-1);

	if (*lineptr == NULL || *n == 0)
	{
		*n = READ_BUF_SIZE;
		*lineptr = malloc(*n);
		if (!*lineptr)
			return (-1);
	}

	while (1)
	{
		if (pos >= size)
		{
			size = read(fd, buf, READ_BUF_SIZE);
			pos = 0;
			if (size <= 0)
				return (i == 0 ? -1 : (ssize_t)i);
		}

		c = buf[pos++];

		if (i >= *n - 1)
		{
			size_t old_size = *n;
			*n += READ_BUF_SIZE;
			new_ptr = malloc(*n);
			if (!new_ptr)
				return (-1);

			memcpy(new_ptr, *lineptr, old_size);
			free(*lineptr);
			*lineptr = new_ptr;
		}

		(*lineptr)[i++] = c;
		if (c == '\n')
			break;
	}
	(*lineptr)[i] = '\0';
	return (i);
}
