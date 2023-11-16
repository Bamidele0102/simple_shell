#include "shell.h"

/**
 * get_history_file - It gets the history file.
 * @context: This is the context struct.
 *
 * Return: The allocated string.
 */
char *get_history_file(ShellContext *context)
{
	char *buf, *dir;

	dir = _getenv(context, "HOME=");
	if (!dir)
		return (NULL);

	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(FILE_HISTORY) + 2));
	if (!buf)
		return (NULL);

	buf[0] = 0;
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, FILE_HISTORY);
	return (buf);
}

/**
 * write_history - It creates a file, or appends to an existing file
 * @context: This is the context struct.
 *
 * Return: 1 on success, -1 otherwise
 */
int write_history(ShellContext *context)
{
	ssize_t fd;
	char *filename = get_history_file(context);
	list_node *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);

	if (fd == -1)
		return (-1);

	node = context->history;
	while (node)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
		node = node->next;
	}

	_putfd(BUFFER_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_history - It reads the history file
 * and builds the history list.
 *
 * @context: This is the context struct.
 *
 * Return: It returns history count on success,
 * 0 otherwise -1 on error
 */
int read_history(ShellContext *context)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_history_file(context);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			build_history_list(context, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_history_list(context, buf + last, linecount++);
	free(buf);
	context->historyCount = linecount;
	while (context->historyCount-- >= MAXIMUM_HISTORY)
		delete_node_at_index(&(context->history), 0);
	renumber_history(context);
	return (context->historyCount);
}

/**
 * build_history_list - This function builds the history list.
 * @context: This is the context struct.
 * @buf: The buffer to store the input.
 * @linecount: The line count of the history list.
 *
 * Return: Always 0
 */
int build_history_list(ShellContext *context, char *buf, int linecount)
{
	list_node *node = NULL;

	if (context->history)
		node = context->history;

	add_node_end(&node, buf, linecount);

	if (!context->history)
		context->history = node;

	return (0);
}

/**
 * renumber_history - This function renumbers the history list.
 * @context: This is the context struct.
 *
 * Return: The number of history list items.
 */
int renumber_history(ShellContext *context)
{
	list_node *node = context->history;
	int i;

	for (i = 0; node; node = node->next)
	{
		node->num = i++;
	}

	return (context->historyCount = i);
}

