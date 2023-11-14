#include "shell.h"

/**
 * hsh - The original shell loop.
 * @info: This is the address of the ShellContext
 struct.
 * @av: This is the arguments passed to the shell.
 *
 * Return: It returns 0 on success, 1 on error, or error code.
 */
int hsh(ShellContext *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	for (; r != -1 && builtin_ret != -2;)
	{
		clear_info(info);
		if (interactive(info))
			_puts("$ ");
		_eputchar(BUFFER_FLUSH);
		r = get_input(info);
		if (r != -1)
		{
			set_info(info, av);
			builtin_ret = find_builtin(info);
			if (builtin_ret == -1)
				find_cmd(info);
		}
		else if (interactive(info))
			_putchar('\n');
		free_info(info, 0);
	}
	write_history(info);
	free_info(info, 1);
	if (!interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->errorNumber == -1)
			exit(info->status);
		exit(info->errorNumber);
	}
	return (builtin_ret);
}

/**
 * find_builtin - It locates a builtin function.
 * @info: This is the address of the ShellContext
 struct.
 *
 * Return: -1 if the builtin is not found,
 *			0 if the builtin executed successfully,
 *			1 if the builtin function is found, but failed, and
 *			-2 if the builtin signals exit().
 */
int find_builtin(ShellContext *info)
{
	int i = 0, builtinReturnValue = -1;
	BuiltinEntry builtinTable[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	while (builtinTable[i].type)
	{
		if (_strcmp(info->argv[0], builtinTable[i].type) == 0)
		{
			info->lineCount++;
			builtinReturnValue = builtinTable[i].function(info);
			break;
		}
		i++;
	}

	return (builtinReturnValue);
}

/**
 * find_cmd - This function locates a command in the path.
 * @info:This is the address of the ShellContext
 struct.
 *
 * Return: Always returns void.
 */
void find_cmd(ShellContext *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->lineCountFlag == 1)
	{
		info->lineCount++;
		info->lineCountFlag = 0;
	}
	i = k = 0;
	while (info->arg[i])
	{
		if (!is_delim(info->arg[i], " \t\n"))
		{
			k++;
		}
		i++;
	}
	if (!k)
		return;

	path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_cmd(info);
	}
	else
	{
		if ((interactive(info) || _getenv(info, "PATH=") ||
					info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
		{
			fork_cmd(info);
		}
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_error(info, "not found\n");
		}
	}
}

/**
 * fork_cmd - This function forks and exec thread to run cmd.
 * @info: This is the address of the ShellContext
 struct.
 *
 * Return: Always returns void.
 */
void fork_cmd(ShellContext *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
			free_info(info, 1);
			if (errno == EACCES)
			{
				exit(126);
			}
			exit(1);
		}
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
			{
				print_error(info, "Permission denied\n");
			}
		}
	}
}

