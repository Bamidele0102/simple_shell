#include "shell.h"

/**
 * get_history_file - It gets the history file.
 * @info: This is the info struct.
 *
 * Return: The allocated string.
 */
char *get_history_file(ShellContext *info)
{
	char *buf, *dir;

	dir = _getenv(info, "HOME=");
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
 * @info: This is the info struct.
 *
 * Return: 1 on success, -1 otherwise
 */
int write_history(ShellContext *info)
{
	ssize_t fd;
	char *filename = get_history_file(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);

	if (fd == -1)
		return (-1);

	node = info->history;
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
 * @info: This is the info struct.
 *
 * Return: It returns history count on success,
 * 0 otherwise -1 on error
 */
int read_history(ShellContext *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_history_file(info);

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
			build_history_list(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_history_list(info, buf + last, linecount++);
	free(buf);
	info->historyCount = linecount;
	while (info->historyCount-- >= MAXIMUM_HISTORY)
		delete_node_at_index(&(info->history), 0);
	renumber_history(info);
	return (info->historyCount);
}

/**
 * build_history_list - This function builds the history list.
 * @info: This is the info struct.
 * @buf: The buffer to store the input.
 * @linecount: The line count of the history list.
 *
 * Return: Always 0
 */
int build_history_list(ShellContext *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;

	add_node_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;

	return (0);
}

/**
 * renumber_history - This function renumbers the history list.
 * @info: This is the info struct.
 *
 * Return: The number of history list items.
 */
int renumber_history(ShellContext *info)
{
	list_t *node = info->history;
	int i;

	for (i = 0; node; node = node->next)
	{
		node->num = i++;
	}

	return (info->historyCount = i);
}

