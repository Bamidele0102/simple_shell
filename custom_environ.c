#include "shell.h"

/**
 * _myenv - It displays the current environment.
 * @context: This is the context struct.
 *
 * Description: This is the function that displays the current environment.
 *
 * Return: Always 0.
 */
int _myenv(ShellContext *context)
{
	print_list_str(context->env);
	return (0);
}

/**
 * _getenv - This function gets the value of an environment variable..
 * @context: This is the context struct.
 * @name: Environment variable name.
 *
 * Return: The value of the environment variable.
 */
char *_getenv(ShellContext *context, const char *name)
{
	list_node *node = context->env;
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
 * @context: This is the context struct.
 *
 * Description: This function initializes a new environment variable
 * or modifies an existing one.
 *
 * Return: Always 0.
 */
int _mysetenv(ShellContext *context)
{
	if (context->argc != 3)
	{
		_eputs("Incorrect number of arguments\n");
		return (1);
	}
	if (_setenv(context, context->argv[1], context->argv[2]))
		return (0);
	return (1);
}

/**
 * _myunsetenv - Is used to remove an environment variable.
 * @context: This is the context struct.
 *
 * Deascrition: This function removes an environment variable.
 *
 * Return: Always 0.
 */
int _myunsetenv(ShellContext *context)
{
	int i = 1;

	if (context->argc == 1)
	{
		_eputs("Too few arguments.\n");
		return (1);
	}
	while (context->argv[i])
	{
		_unsetenv(context, context->argv[i]);
		i++;
	}

	return (0);
}

/**
 * populate_env_list - It populates the env linked list.
 * @context: This is the context struct.
 *
 * Description: This function populates the env linked list.
 *
 * Return: Always 0.
 */
int populate_env_list(ShellContext *context)
{
	list_node *node = NULL;
	size_t i = 0;

	while (environ[i])
	{
		add_node_end(&node, environ[i], 0);
		i++;
	}
	context->env = node;
	return (0);
}

