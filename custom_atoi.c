#include "shell.h"

/**
 * interactive - This will check if the shell is in interactive mode.
 * @info: The pointer to the ShellContext structure.
 *
 * Return: Always 1 if in interactive mode, 0 otherwise.
 */
int interactive(ShellContext *info)
{
	return (isatty(STDIN_FILENO) && info->input_fd <= 2);
}

/**
 * is_delim - This will check if a character is a delimiter.
 * @c: Checks the character c.
 * @delim: The delimiter to check.
 *
 * Return: Always 1 if the character is a delimiter, 0 otherwise.
 */
int is_delim(char c, const char *delim)
{
	while (*delim)
	{
		if (*delim == c)
		{
			return (1);
		}
		delim++;
	}
	return (0);
}

/**
 * isalpha - This will check if a character is alphabetic.
 * @c: It will check the character c.
 *
 * Return: Always 1 if the character is alphabetic, 0 otherwise.
 */
int isalpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

/**
 * atoi - This will convert a string to an integer.
 * @str: It will convert the string s to an integer.
 *
 * Return: The integer value of the string.
 */
int atoi(const char *str)
{
	int sign = 1, output = 0;
	int i = 0;

	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}

	while (str[i] >= '0' && str[i] <= '9')
	{
		output = output * 10 + (str[i] - '0');
		i++;
	}

	return (sign * output);
}
