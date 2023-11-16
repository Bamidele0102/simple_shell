#include "shell.h"

/**
 * is_chain - This function tests if a string is a chain delimiter
 * @context: This is the parameter struct.
 * @buffs: This is the char buffer.
 * @p: The address of current position in buffs.
 *
 * Return: 1 if found, 0 if not
 */
int is_chain(ShellContext *context, char *buffs, size_t *p)
{
	size_t j = *p;

	if (buffs[j] == '|' && buffs[j + 1] == '|')
	{
		buffs[j] = 0;
		j++;
		context->commandBufferType = LOGIC_OR_CMD;
	}
	else if (buffs[j] == '&' && buffs[j + 1] == '&')
	{
		buffs[j] = 0;
		j++;
		context->commandBufferType = LOGIC_AND_CMD;
	}
	else if (buffs[j] == ';') /* found end of this command */
	{
		buffs[j] = 0; /* replace semicolon with null */
		context->commandBufferType = CHAINING_CMD;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * eval_chain - This function checks if a chain is satisfied
 * @context: This is the parameter struct.
 * @buffs: the char buffer
 * @p: The address of current position in buffs.
 * @i: This is the index of the command in the command buffer.
 * @len: This is the length of the command buffer.
 *
 * Return: Always void.
 */
void eval_chain(ShellContext *context, char *buffs, size_t *p,
		size_t i, size_t len)
{
	size_t j = *p;

	if (context->commandBufferType == LOGIC_AND_CMD)
	{
		if (context->status)
		{
			buffs[i] = 0;
			j = len;
		}
	}
	if (context->commandBufferType == LOGIC_OR_CMD)
	{
		if (!context->status)
		{
			buffs[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * replace_alias - This function replaces aliases in the tokenized string
 * @context: This is the parameter struct.
 *
 * Return: This returns 1 if replaced, 0 otherwise.
 */
int replace_alias(ShellContext *context)
{
	int i;
	list_node *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(context->alias, context->argv[0], '=');
		if (!node)
			return (0);
		free(context->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		context->argv[0] = p;
	}
	return (1);
}

/**
 * replace_vars - This function replaces variables in the tokenized string
 * @context: This is the parameter struct.
 *
 * Return: This returns 1 if replaced, 0 otherwise.
 */
int replace_vars(ShellContext *context)
{
	int i = 0;
	list_node *node;

	while (context->argv[i])
	{
		if (context->argv[i][0] != '$' || !context->argv[i][1])
		{
			i++;
			continue;
		}

		if (!_strcmp(context->argv[i], "$?"))
		{
			replace_string(&(context->argv[i]),
					_strdup(int_to_base(context->status, 10, 0)));
		}
		else if (!_strcmp(context->argv[i], "$$"))
		{
			replace_string(&(context->argv[i]),
					_strdup(int_to_base(getpid(), 10, 0)));
		}
		else
		{
			node = node_starts_with(context->env, &context->argv[i][1], '=');
			if (node)
			{
				replace_string(&(context->argv[i]),
						_strdup(_strchr(node->str, '=') + 1));
			}
			else
			{
				replace_string(&context->argv[i], _strdup(""));
			}
		}

		i++;
	}

	return (0);
}

/**
 * replace_string - This function replaces a string with another string.
 * @old: This is the address of the old string.
 * @new: This is the address of the new string.
 *
 * Return: This returns 1 if replaced, 0 otherwise.
 */
int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
