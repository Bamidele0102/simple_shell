#include "shell.h"

/**
 * _erratoi - This converts a string to an integer.
 * @s: The string to be converted.
 *
 * Return: On success, the integer value of the string.
 * On error, -1 is returned,
 *
 */
int _erratoi(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;

	while (s[i] != '\0')
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);

		i++;
	}

	return (result);
}

/**
 * print_error - It displays the error message.
 * @info: This is the parameter & return info struct
 * @estr: The string error type
 * Return: void
 */
void print_error(ShellContext *info, char *estr)
{
	_eputs(info->fileName);
	_eputs(": ");
	print_d(info->lineCount, STDERR_FILENO);
	_eputs(": ");
	_eputs(info->argv[0]);
	_eputs(": ");
	_eputs(estr);
}

/**
 * print_d - This displays a decimal (integer), and number (base 10)
 * @input: The input to be displayed.
 * @fd: This is the filedescriptor.
 *
 * Return: ... the number of characters displayed.
 */
int print_d(int input, int fd)
{
	int (*__putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int _abs_, current;

	if (fd == STDERR_FILENO)
		__putchar = _eputchar;

	if (input < 0)
	{
		_abs_ = -input;
		__putchar('-');
		count++;
	}
	else
		_abs_ = input;

	current = _abs_;

	for (i = 1000000000; i > 1; i /= 10)
	{
		if (_abs_ / i)
		{
			__putchar('0' + current / i);
			count++;
		}
		current %= i;
	}

	__putchar('0' + current);
	count++;

	return (count);
}

/**
 * convert_number - This is a clone of itoa for converting numbers.
 * @num: For number.
 * @base: For base.
 * @flags: This is the argument flags.
 *
 * Return: ... string
 */
char *convert_number(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & TO_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';
	}

	array = flags & TO_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do {
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;

	return (ptr);
}

/**
 * remove_comments - It replaces first instance of '#' with '\0'
 * @buf: Modified string address.
 *
 * Return: Always 0;
 */
void remove_comments(char *buf)
{
	int i = 0;

	while (buf[i] != '\0')
	{
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
		i++;
	}
}
