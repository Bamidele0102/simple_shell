#include "shell.h"

/**
 * hsh - The original shell loop.
 * @context: This is the address of the ShellContext struct.
 * @av: This is the arguments passed to the shell.
 *
 * Return: It returns 0 on success, 1 on error, or error code.
 */
int hsh(ShellContext *context, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	for (; r != -1 && builtin_ret != -2;)
	{
		clear_context(context);
		if (interactive(context))
			_puts("$ ");
		_eputchar(BUFFER_FLUSH);
		r = get_input(context);
		if (r != -1)
		{
			set_context(context, av);
			builtin_ret = find_builtin(context);
			if (builtin_ret == -1)
				find_cmd(context);
		}
		else if (interactive(context))
			_putchar('\n');
		free_context(context, 0);
	}
	write_history(context);
	free_context(context, 1);
	if (!interactive(context) && context->status)
		exit(context->status);
	if (builtin_ret == -2)
	{
		if (context->errorNumber == -1)
			exit(context->status);
		exit(context->errorNumber);
	}
	return (builtin_ret);
}

/**
 * find_builtin - It locates a builtin function.
 * @context: This is the address of the ShellContext struct.
 *
 * Return: -1 if the builtin is not found,
 *			0 if the builtin executed successfully,
 *			1 if the builtin function is found, but failed, and
 *			-2 if the builtin signals exit().
 */
int find_builtin(ShellContext *context)
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
		if (_strcmp(context->argv[0], builtinTable[i].type) == 0)
		{
			context->lineCount++;
			builtinReturnValue = builtinTable[i].function(context);
			break;
		}
		i++;
	}

	return (builtinReturnValue);
}

/**
 * find_cmd - This function locates a command in the path.
 * @context:This is the address of the ShellContext struct.
 *
 * Return: Always returns void.
 */
void find_cmd(ShellContext *context)
{
	char *path = NULL;
	int i, k;

	context->path = context->argv[0];
	if (context->lineCountFlag == 1)
	{
		context->lineCount++;
		context->lineCountFlag = 0;
	}
	i = k = 0;
	while (context->arg[i])
	{
		if (!is_delim(context->arg[i], " \t\n"))
		{
			k++;
		}
		i++;
	}
	if (!k)
		return;

	path = find_path(context, _getenv(context, "PATH="), context->argv[0]);
	if (path)
	{
		context->path = path;
		fork_cmd(context);
	}
	else
	{
		if ((interactive(context) || _getenv(context, "PATH=") ||
					context->argv[0][0] == '/') && is_cmd(context, context->argv[0]))
		{
			fork_cmd(context);
		}
		else if (*(context->arg) != '\n')
		{
			context->status = 127;
			display_error(context, "not found\n");
		}
	}
}

/**
 * fork_cmd - This function forks and exec thread to run cmd.
 * @context: This is the address of the ShellContext struct.
 *
 * Return: Always returns void.
 */
void fork_cmd(ShellContext *context)
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
		if (execve(context->path, context->argv, get_environ(context)) == -1)
		{
			free_context(context, 1);
			if (errno == EACCES)
			{
				exit(126);
			}
			exit(1);
		}
	}
	else
	{
		wait(&(context->status));
		if (WIFEXITED(context->status))
		{
			context->status = WEXITSTATUS(context->status);
			if (context->status == 126)
			{
				display_error(context, "Permission denied\n");
			}
		}
	}
}

