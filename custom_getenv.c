#include "shell.h"

/**
 * get_environ - Get the environment variables.
 * @info: The info struct.
 *
 * Return: Always 0
 */
char **get_environ(ShellContext *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _unsetenv - It deletes an environment variable.
 * @info: The info struct.
 *
 * @var: The var to be deleted.
 * Return: 1 on delete, 0 otherwise
 */
int _unsetenv(ShellContext *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	for (; node; node = node->next, i++)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = delete_node_at_index(&(info->env), i);
			i = 0;
			node = info->env;
		}
	}
	return (info->env_changed);
}

/**
 * _setenv - It sets an environment variable.
 *
 * @info: The info struct.
 *
 * @var: The var to be set.
 * @value: The value of the var.
 * Return: Always 0.
 */
int _setenv(ShellContext *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);

	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);

	for (node = info->env; node; node = node->next)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
	}
	add_node_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}

