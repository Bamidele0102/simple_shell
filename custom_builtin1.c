#include "shell.h"

/**
 * _myhistory - This function displays the history list.
 * @context: The context struct.
 *
 * Description: This function displays the history list.
 *
 * Return: Always 0 on success, 1 on error.
 */
int _myhistory(ShellContext *context)
{
	print_list(context->history);
	return (0);
}

/**
 * unset_alias - This function removes an alias.
 * @context: The parameter context struct.
 * @aliasString: This is the alias string to be removed.
 *
 * Description: This function removes an alias from the alias list.
 *
 * Return: Always 0 on success, 1 on error.
 */
int unset_alias(ShellContext *context, char *aliasString)
{
	char *p, c;
	int deletion_result;

	p = _strchr(aliasString, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	deletion_result = delete_node_at_index(&(context->alias),
			get_node_index(context->alias,
			node_starts_with(context->alias, aliasString, -1)));
	*p = c;
	return (deletion_result);
}

/**
 * set_alias - It sets or updates an alias.
 * @context: The parameter context struct.
 * @aliasString: This is the alias string.
 *
 * Description: This function sets or updates an alias.
 *
 * Return: Always 0 on success, 1 on error.
 */
int set_alias(ShellContext *context, char *aliasString)
{
	char *p;

	p = _strchr(aliasString, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(context, aliasString));

	unset_alias(context, aliasString);
	return (add_node_end(&(context->alias), aliasString, 0) == NULL);
}

/**
 * print_alias - The function displays an alias.
 * @node: This is the node to be printed.
 *
 * Description: This function displays an alias.
 *
 * Return: Always 0 on success, 1 on error.
 */
int print_alias(list_node *node)
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
 * @context: The struct context.
 *
 * Description: The function mimics the alias builtin by either display aliases
 * or setting/updating aliases based on the arguments provided.
 *
 * Return: Always 0 on success, 1 on error.
 */
int _myalias(ShellContext *context)
{
	int i = 0;
	char *p = NULL;
	list_node *node = NULL;

	if (context->argc == 1)
	{
		for (node = context->alias; node; node = node->next)
			print_alias(node);
		return (0);
	}

	for (i = 1; context->argv[i]; i++)
	{
		p = _strchr(context->argv[i], '=');
		if (p)
			set_alias(context, context->argv[i]);
		else
			print_alias(node_starts_with(context->alias, context->argv[i], '='));
	}

	return (0);
}

