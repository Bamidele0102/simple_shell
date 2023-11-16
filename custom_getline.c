#include "shell.h"

/**
 * input_buf - This function reads the input from the user.
 * @context: This is the parameter & return context struct
 * @buffs: The buffer to store the input
 * @len: The length of the buffer
 *
 * Return: The number of bytes read.
 */
ssize_t input_buf(ShellContext *context, char **buffs, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* If nothing left in the buffer, fill it */
	{
		free(*buffs);
		*buffs = NULL;
		signal(SIGINT, sigint_handler);
#if ENABLE_CUSTOM_GETLINE
		r = getline(buffs, &len_p, stdin);
#else
		r = _getline(context, buffs, &len_p);
#endif
		if (r > 0)
		{
			if ((*buffs)[r - 1] == '\n')
			{
				(*buffs)[r - 1] = '\0'; /* Remove trailing newline */
				r--;
			}
			context->lineCountFlag = 1;
			delete_comments(*buffs);
			build_history_list(context, *buffs, context->historyCount++);
			*len = r;
			context->commandBuffer = buffs;
		}
	}
	return (r);
}

/**
 * get_input - This function reads the input from the user.
 * @context: This is the parameter & return context struct
 *
 * Return: The number of bytes read.
 */
ssize_t get_input(ShellContext *context)
{
	static char *buffs; /* The ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(context->arg), *p;

	_putchar(BUFFER_FLUSH);
	r = input_buf(context, &buffs, &len);
	if (r == -1) /* EOF */
		return (-1);

	if (len)
	{ /* We have commands left in the chain buffer */
		j = i; /* Init new iterator to the current buffs position */
		p = buffs + i; /* Get pointer for return */

		eval_chain(context, buffs, &j, i, len);
		for (; j < len; j++)
		{ /* Iterate to semicolon or end */
			if (is_chain(context, buffs, &j))
				break;
		}

		i = j + 1; /* Increment past nulled ';' */
		if (i >= len) /* Reached the end of the buffer? */
		{
			i = len = 0; /* Reset position and length */
			context->commandBufferType = NORMAL_CMD;
		}

		*buf_p = p; /* Pass back pointer to the current command position */
		return (_strlen(p)); /* Return length of the current command */
	}

	*buf_p = buffs; /* Else not a chain, pass back buffer from _getline() */
	return (r); /* Return length of buffer from _getline() */
}

/**
 * read_buf - This function reads the input from the user.
 * @context: This is the parameter & return context struct.
 * @buffs: The buffer to store the input.
 * @i: The iterator.
 *
 * Return: The number of bytes read.
 */
ssize_t read_buf(ShellContext *context, char *buffs, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(context->input_fd, buffs, READ_THEBUFF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _getline - This gets the next line of input from STDIN.
 * @context: This is the parameter & return context struct
 * @ptr: This is the address of pointer to buffer, preallocated or NULL
 * @length: The size of preallocated ptr buffer if not NULL
 *
 * Return: The number of bytes read.
 */
int _getline(ShellContext *context, char **ptr, size_t *length)
{
	static char buffs[READ_THEBUFF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(context, buffs, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buffs + i, '\n');
	k = c ? 1 + (unsigned int)(c - buffs) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? (free(p), -1) : -1);

	if (s)
		_strncat(new_p, buffs + i, k - i);
	else
		_strncpy(new_p, buffs + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigint_handler - It handles the SIGINT signal.
 * @sig_num: The signal number.
 *
 * Return: This returns void.
 */
void sigint_handler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	fflush(stdout); /* Flush the standard output */
	_putchar(BUFFER_FLUSH);
}

