#include "shell.h"

/**
 * _strcpy - copies a string
 * @dest: the destination
 * @src: the source
 *
 * Return: pointer to destination
 */
char *_strcpy(char *dest, char *src)
{
	int i = 0;

	if (dest == src || src == NULL)
		return (dest);

	/* Copy characters from source to destination using a for loop */
	for (i = 0; src[i]; i++)
		dest[i] = src[i];

	dest[i] = '\0'; /* Add null terminator to the destination */
	return (dest);
}

/**
 * _strdup - duplicates a string
 * @str: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
char *_strdup(const char *str)
{
	int length = 0;
	char *ret;

	if (str == NULL)
		return (NULL);

	/* Calculate the length of the string using a while loop */
	while (str[length])
		length++;

	/* Allocate memory for the duplicated string */
	ret = malloc(sizeof(char) * (length + 1));
	if (!ret)
		return (NULL);

	/* Copy characters from the original string to the duplicated string */
	for (length = 0; str[length]; length++)
		ret[length] = str[length];

	ret[length] = '\0'; /* Add null terminator to the duplicated string */
	return (ret);
}

/**
 * _puts - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void _puts(char *str)
{
	int i = 0;

	if (!str)
		return;

	/* Print characters in the string until null terminator is encountered */
	for (; str[i] != '\0'; i++)
	{
		_putchar(str[i]);
	}
}

/**
 * _putchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putchar(char c)
{
	static int i;
	static char buffs[WRITE_THEBUFF_SIZE];

	if (c == BUFFER_FLUSH || i >= WRITE_THEBUFF_SIZE)
	{
		write(1, buffs, i);
		i = 0;
	}

	if (c != BUFFER_FLUSH)
		buffs[i++] = c;

	return (1);
}
