#include "shell.h"

/**
 * string_to_int - This converts a string to an integer.
 * @str: The string to be converted.
 *
 * Return: On success, the integer value of the string.
 * On error, -1 is returned,
 *
 */
int string_to_int(char *str)
{
	int i = 0;
	unsigned long int result = 0;

	if (*str == '+')
		str++;

	while (str[i] != '\0')
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			result *= 10;
			result += (str[i] - '0');
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
 * display_error - It displays the error message.
 * @context: This is the parameter & return context struct
 * @error_type: The string error type
 * Return: void
 */
void display_error(ShellContext *context, char *error_type)
{
	_eputs(context->fileName);
	_eputs(": ");
	display_decimal(context->lineCount, STDERR_FILENO);
	_eputs(": ");
	_eputs(context->argv[0]);
	_eputs(": ");
	_eputs(error_type);
}

/**
 * display_decimal - This displays a decimal (integer), and number (base 10)
 * @number: The number to be displayed.
 * @fd: This is the filedescriptor.
 *
 * Return: ... the number of characters displayed.
 */
int display_decimal(int number, int fd)
{
	int (*__putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int abs_value, current;

	if (fd == STDERR_FILENO)
		__putchar = _eputchar;

	if (number < 0)
	{
		abs_value = -number;
		__putchar('-');
		count++;
	}
	else
		abs_value = number;

	current = abs_value;

	for (i = 1000000000; i > 1; i /= 10)
	{
		if (abs_value / i)
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
 * int_to_base - This converts a number to a string representation.
 * @num: For number.
 * @base: For base.
 * @flags: This is the argument flags.
 *
 * Return: ... string
 */
char *int_to_base(long int num, int base, int flags)
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
 * delete_comments - It replaces first instance of '#' with '\0'
 * @buffs: Modified string address.
 *
 * Return: Always 0;
 */
void delete_comments(char *buffs)
{
	int i = 0;

	while (buffs[i] != '\0')
	{
		if (buffs[i] == '#' && (!i || buffs[i - 1] == ' '))
		{
			buffs[i] = '\0';
			break;
		}
		i++;
	}
}
