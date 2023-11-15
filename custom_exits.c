#include "shell.h"

/**
 * _strncpy - It copies a string.
 * @dest: This is the destination string to be copied to.
 * @src: This is the source string.
 * @n: n characters to be copied.
 * Return: ... the interconnected string
 */
char *_strncpy(char *dest, char *src, int n)
{
	int i = 0;
	char *s = dest;

	for (i = 0; i < n - 1 && src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}

	if (i < n)
	{
		int j;

		for (j = i; j < n; j++)
		{
			dest[j] = '\0';
		}
	}

	return (s);
}

/**
 * _strncat - The string concatenation function.
 * @dest: This is the destination string.
 * @src: This is the source string.
 * @n: n number of characters to be concatenated.
 * Return: ... the interconnected string
 */
char *_strncat(char *dest, char *src, int n)
{
	int i = 0;
	int j = 0;
	char *s = dest;

	while (dest[i] != '\0')
	{
		i++;
	}

	for (; j < n && src[j] != '\0'; j++)
	{
		dest[i] = src[j];
		i++;
	}

	if (j < n)
	{
		dest[i] = '\0';
	}

	return (s);
}

/**
 * _strchr - This function searches for a character in a string.
 * @s: This is the string to be searched.
 * @c: This is the character to be searched for.
 * Return: (s) is the memory address of the first occurrence of c in s,
 * or NULL if c is not found.
 */
char *_strchr(char *s, char c)
{
	do {
		if (*s == c)
		{
			return (s);
		}
		s++;
	} while (*s != '\0');

	return (NULL);
}

