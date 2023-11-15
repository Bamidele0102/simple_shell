#include "shell.h"

/**
 * is_cmd - Checks if a file is a command.
 * @info: This is the address of the ShellContext
 struct.
 * @path: This is the path to the file.
 *
 * Return: 1 if the file is a command, 0 otherwise.
 */
int is_cmd(ShellContext *info, char *path)
{
	struct stat st;

	(void)info;

	/* Check if the path is valid and obtain file information */
	if (!path || stat(path, &st))
		return (0);

	/* Check if the file is a regular file */
	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * dup_chars - Duplicates characters from a string.
 * @pathstr: The path to string to duplicate.
 * @start: This is the starting index.
 * @stop: This is the stopping index.
 *
 * Return: ...the pointer to the newly allocated string.
 */
char *dup_chars(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int i, k;

	/* Copy characters from start to stop (excluding ':') into buf */
	for (k = 0, i = start; k < 1024 && i < stop; i++)
	{
		if (pathstr[i] != ':')
			buf[k++] = pathstr[i];
	}

	/* Null-terminate the buffer */
	buf[k] = '\0';

	return (buf);
}

/**
 * find_path - This function finds the full path of a command.
 * @info: This is the address of the ShellContext
 struct.
 * @pathstr: The PATH string.
 * @cmd: The command to find.
 *
 * Return: Full path of cmd if found or NULL.
 */
char *find_path(ShellContext *info, char *pathstr, char *cmd)
{
	int curr_pos = 0;
	int i;

	/* Check for special case where cmd starts with "./" */
	if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
	{
		if (is_cmd(info, cmd))
			return (cmd);
	}

	/* Loop through the PATH string */
	for (i = 0; ; i++)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			/* Duplicate characters from curr_pos to i into path */
			char *path = dup_chars(pathstr, curr_pos, i);

			/* Concatenate cmd to path */
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}

			/* Check if the path is a valid command */
			if (is_cmd(info, path))
				return (path);

			/* Break if the end of pathstr is reached */
			if (!pathstr[i])
				break;

			/* Update curr_pos for the next iteration */
			curr_pos = i + 1;
		}
	}
	return (NULL);
}
