#include "shell.h"

/**
 * _myhistory - This function displays the history list.
 * @info: The info struct.
 *
 * Description: This function displays the history list.
 *
 * Return: Always 0 on success, 1 on error.
 */
int _myhistory(ShellContext *info)
{
	print_list(info->history);
	return (0);
}

/**
 * unset_alias - This function removes an alias.
 * @info: The parameter info struct.
 * @str: This is the alias string.
 *
 * Description: This function removes an alias from the alias list.
 *
 * Return: Always 0 on success, 1 on error.
 */
int unset_alias(ShellContext *info, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(info->alias),
			get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - It sets or updates an alias.
 * @info: The parameter info struct.
 * @str: This is the alias string.
 *
 * Description: This function sets or updates an alias.
 *
 * Return: Always 0 on success, 1 on error.
 */
int set_alias(ShellContext *info, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, str));

	unset_alias(info, str);
	return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - The function displays an alias.
 * @node: This is the node to be printed.
 *
 * Description: This function displays an alias.
 *
 * Return: Always 0 on success, 1 on error.
 */
int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalias - It mimics the alias builtin.(man 1 alias)
 * @info: The struct info.
 *
 * Description: The function mimics the alias builtin by either display aliases
 * or setting/updating aliases based on the arguments provided.
 *
 * Return: Always 0 on success, 1 on error.
 */
int _myalias(ShellContext *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		for (node = info->alias; node; node = node->next)
			print_alias(node);
		return (0);
	}

	for (i = 1; info->argv[i]; i++)
	{
		p = _strchr(info->argv[i], '=');
		if (p)
			set_alias(info, info->argv[i]);
		else
			print_alias(node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}

