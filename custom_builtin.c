#include "shell.h"

/**
 * _myexit - This exits the shell and or sets an error number
 * @info: The info struct.
 *
 * This function exits the shell and or sets an error number. If an exit number
 *
 * Return: 0 on success, 1 on error.
 */
int _myexit(ShellContext *info)
{
	int exitcheck;

	if (info->argv[1])
	{
		exitcheck = _erratoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			print_error(info, "Illegal number: ");
			_eputs(info->argv[1]);
			_eputchar('\n');
			return (1);
		}
		info->errorNumber = exitcheck;
		return (-2);
	}
	info->errorNumber = -1;
	return (-2);
}

/**
 * _mycd - This changes the current directory of the process.
 * @info: The info struct.
 *
 * This function changes the current directory of the process. If the argument
 *
 * Return: 0 on success, 1 on error.
 */
int _mycd(ShellContext *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = _getenv(info, "HOME=");
		if (!dir)
			chdir_ret = chdir((dir = _getenv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		if (!_getenv(info, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(info, "OLDPWD="));
		_putchar('\n');
		chdir_ret = chdir((dir = _getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(info, "can't cd to ");
		_eputs(info->argv[1]);
		_eputchar('\n');
	}
	else
	{
		_setenv(info, "OLDPWD", _getenv(info, "PWD="));
		_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelp - This displays a help message.
 * @info: The info struct.
 *
 * This function displays a help message. If the argument
 *
 * Return: 0 on success, 1 on error.
 */
int _myhelp(ShellContext *info)
{
	char **arg_array;

	arg_array = info->argv;
	_puts("help call works. Function not yet implemented\n");
	if (0)
		_puts(*arg_array);
	return (0);
}
