#include "shell.h"

/**
 * _myenv - It displays the current environment.
 * @info: This is the info struct.
 *
 * Deacription: This is the function that displays the current environment.
 *
 * Return: Always 0.
 */
int _myenv(ShellContext *info)
{
	print_list_str(info->env);
	return (0);
}

/**
 * _getenv - This function gets the value of an environment variable..
 * @info: This is the info struct.
 * @name: Environment variable name.
 *
 * Return: The value of the environment variable.
 */
char *_getenv(ShellContext *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _mysetenv - Initializes a new env variable or modifies an existing one.
 * @info: This is the info struct.
 *
 * Description: This function initializes a new environment variable
 * or modifies an existing one.
 *
 * Return: Always 0.
 */
int _mysetenv(ShellContext *info)
{
	if (info->argc != 3)
	{
		_eputs("Incorrect number of arguments\n");
		return (1);
	}
	if (_setenv(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * _myunsetenv - Is used to remove an environment variable.
 * @info: This is the info struct.
 *
 * Deascrition: This function removes an environment variable.
 *
 * Return: Always 0.
 */
int _myunsetenv(ShellContext *info)
{
	int i = 1;

	if (info->argc == 1)
	{
		_eputs("Too few arguments.\n");
		return (1);
	}
	while (info->argv[i])
	{
		_unsetenv(info, info->argv[i]);
		i++;
	}

	return (0);
}

/**
 * populate_env_list - It populates the env linked list.
 * @info: This is the info struct.
 *
 * Description: This function populates the env linked list.
 *
 * Return: Always 0.
 */
int populate_env_list(ShellContext *info)
{
	list_t *node = NULL;
	size_t i = 0;

	while (environ[i])
	{
		add_node_end(&node, environ[i], 0);
		i++;
	}
	info->env = node;
	return (0);
}

