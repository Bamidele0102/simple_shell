#include "shell.h"

/**
 * _myexit - This exits the shell and or sets an error number
 * @context: The context struct.
 *
 * This function exits the shell and or sets an error number. If an exit number
 *
 * Return: 0 on success, 1 on error.
 */
int _myexit(ShellContext *context)
{
	int exitcheck;

	if (context->argv[1])
	{
		exitcheck = string_to_int(context->argv[1]);
		if (exitcheck == -1)
		{
			context->status = 2;
			display_error(context, "Illegal number: ");
			_eputs(context->argv[1]);
			_eputchar('\n');
			return (1);
		}
		context->errorNumber = exitcheck;
		return (-2);
	}
	context->errorNumber = -1;
	return (-2);
}

/**
 * _mycd - This changes the current directory of the process.
 * @context: The context struct.
 *
 * This function changes the current directory of the process. If the argument
 *
 * Return: 0 on success, 1 on error.
 */
int _mycd(ShellContext *context)
{
	char *curr_dir, *new_dir, buffer[1024];
	int chdir_resut;

	curr_dir = getcwd(buffer, 1024);
	if (!curr_dir)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!context->argv[1])
	{
		new_dir = _getenv(context, "HOME=");
		if (!new_dir)
			chdir_resut = chdir((new_dir = _getenv(context, "PWD=")) ? new_dir : "/");
		else
			chdir_resut = chdir(new_dir);
	}
	else if (_strcmp(context->argv[1], "-") == 0)
	{
		if (!_getenv(context, "OLDPWD="))
		{
			_puts(curr_dir);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(context, "OLDPWD="));
		_putchar('\n');
		chdir_resut = chdir((new_dir = _getenv(context, "OLDPWD=")) ? new_dir : "/");
	}
	else
		chdir_resut = chdir(context->argv[1]);
	if (chdir_resut == -1)
	{
		display_error(context, "can't cd to ");
		_eputs(context->argv[1]);
		_eputchar('\n');
	}
	else
	{
		_setenv(context, "OLDPWD", _getenv(context, "PWD="));
		_setenv(context, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelp - This displays a help message.
 * @context: The context struct.
 *
 * This function displays a help message. If the argument
 *
 * Return: 0 on success, 1 on error.
 */
int _myhelp(ShellContext *context)
{
	char **arg_array;

	arg_array = context->argv;
	_puts("help call works. Function not yet implemented\n");
	if (0)
		_puts(*arg_array);
	return (0);
}
