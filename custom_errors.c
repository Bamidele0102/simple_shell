#include "shell.h"

/**
 * _eputs - This displays an input string to stderr.
 * @str: The string to be displayed.
 *
 * Return: Nothing.
 */
void _eputs(char *str)
{
	int i;

	if (!str)
		return;

	for (; str[i] != '\0'; i++)
	{
		_eputchar(str[i]);
	}
}

/**
 * _eputchar - This displays the character c to stderr.
 * @c: The character to be displayed.
 *
 * Return: On success 1. On error, -1 is returned, and errno is set correctly
 */
int _eputchar(char c)
{
	static int i;
	static char buf[WRITE_THEBUFF_SIZE];

	if (c == BUFFER_FLUSH || i >= WRITE_THEBUFF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}

	if (c != BUFFER_FLUSH)
		buf[i++] = c;

	return (1);
}

/**
 * _putfd - This displays the character c to the file descriptor provided.
 * @c: The character to be displayed.
 * @fd: This is the file descriptor.
 *
 * Return: On success 1. On error, -1 is returned, and errno is set correctly.
 */
int _putfd(char c, int fd)
{
	static int i;
	static char buf[WRITE_THEBUFF_SIZE];

	if (c == BUFFER_FLUSH || i >= WRITE_THEBUFF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}

	if (c != BUFFER_FLUSH)
		buf[i++] = c;

	return (1);
}

/**
 * _putsfd - This displays an input string to the file descriptor provided.
 * @str: The string to be displayed.
 * @fd: This is the file descriptor.
 *
 * Return: It returns the number of character written.
 */
int _putsfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);

	for (; str[i] != '\0'; i++)
	{
		_putfd(str[i], fd);
	}

	return (i);
}
