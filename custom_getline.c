#include "shell.h"

/**
 * input_buf - This function reads the input from the user.
 * @info: This is the parameter & return info struct
 * @buf: The buffer to store the input
 * @len: The length of the buffer
 *
 * Return: The number of bytes read.
 */
ssize_t input_buf(ShellContext *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* If nothing left in the buffer, fill it */
	{
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigint_handler);
#if ENABLE_CUSTOM_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = _getline(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* Remove trailing newline */
				r--;
			}
			info->lineCountFlag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->historyCount++);
			*len = r;
			info->commandBuffer = buf;
		}
	}
	return (r);
}

/**
 * get_input - This function reads the input from the user.
 * @info: This is the parameter & return info struct
 *
 * Return: The number of bytes read.
 */
ssize_t get_input(ShellContext *info)
{
	static char *buf; /* The ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	_putchar(BUFFER_FLUSH);
	r = input_buf(info, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);

	if (len)
	{ /* We have commands left in the chain buffer */
		j = i; /* Init new iterator to the current buf position */
		p = buf + i; /* Get pointer for return */

		eval_chain(info, buf, &j, i, len);
		for (; j < len; j++)
		{ /* Iterate to semicolon or end */
			if (is_chain(info, buf, &j))
				break;
		}

		i = j + 1; /* Increment past nulled ';' */
		if (i >= len) /* Reached the end of the buffer? */
		{
			i = len = 0; /* Reset position and length */
			info->commandBufferType = NORMAL_CMD;
		}

		*buf_p = p; /* Pass back pointer to the current command position */
		return (_strlen(p)); /* Return length of the current command */
	}

	*buf_p = buf; /* Else not a chain, pass back buffer from _getline() */
	return (r); /* Return length of buffer from _getline() */
}

/**
 * read_buf - This function reads the input from the user.
 * @info: This is the parameter & return info struct.
 * @buf: The buffer to store the input.
 * @i: The iterator.
 *
 * Return: The number of bytes read.
 */
ssize_t read_buf(ShellContext *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(info->input_fd, buf, READ_THEBUFF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _getline - This gets the next line of input from STDIN.
 * @info: This is the parameter & return info struct
 * @ptr: This is the address of pointer to buffer, preallocated or NULL
 * @length: The size of preallocated ptr buffer if not NULL
 *
 * Return: The number of bytes read.
 */
int _getline(ShellContext *info, char **ptr, size_t *length)
{
	static char buf[READ_THEBUFF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? (free(p), -1) : -1);

	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);

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

