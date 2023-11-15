#include "shell.h"

/**
 * _strlen - This function counts the length of a string.
 * @s: The string to count the length of.
 *
 * Return: The integer length of the string.
 */
int _strlen(char *s)
{
	int i = 0;

	if (!s)
		return (0);

	for (; *s++; i++)
		;

	return (i);
}

/**
 * _strcmp - This function compares two strings.
 * @s1: This is the first string.
 * @s2: This is the second string.
 *
 * Return: Should return negative if s1 < s2,
 *         Should return positive if s1 > s2,
 *         Should zero if s1 == s2.
 */
int _strcmp(char *s1, char *s2)
{
	for (; *s1 && *s2; s1++, s2++)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
	}

	return (*s1 - *s2);
}

/**
 * starts_with - This function checks if a string starts with another string.
 * @haystack: This is the string to check.
 * @needle: This is the substring to check.
 *
 * Return: The address of the first character of the substring if found,
 *         or NULL.
 */
char *starts_with(const char *haystack, const char *needle)
{
	for (; *needle; needle++, haystack++)
	{
		if (*needle != *haystack)
			return (NULL);
	}

	return ((char *)haystack);
}

/**
 * _strcat - This function concatenates two strings.
 * @dest: This is the destination buffer.
 * @src: While, this is the source buffer.
 *
 * Return: The pointer to address of the destination buffer.
 */
char *_strcat(char *dest, char *src)
{
	char *ret = dest;

	for (; *dest; dest++)
		;

	for (; (*dest = *src); dest++, src++)
		;

	return (ret);
}
