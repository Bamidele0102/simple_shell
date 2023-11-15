#include "shell.h"

/**
 * is_chain - This function tests if a string is a chain delimiter
 * @info: This is the parameter struct.
 * @buf: This is the char buffer.
 * @p: The address of current position in buf.
 *
 * Return: 1 if found, 0 if not
 */
int is_chain(ShellContext *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->commandBufferType = LOGIC_OR_CMD;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->commandBufferType = LOGIC_AND_CMD;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		info->commandBufferType = CHAINING_CMD;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * eval_chain - This function checks if a chain is satisfied
 * @info: This is the parameter struct.
 * @buf: the char buffer
 * @p: The address of current position in buf.
 * @i: This is the index of the command in the command buffer.
 * @len: This is the length of the command buffer.
 *
 * Return: Always void.
 */
void eval_chain(ShellContext *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->commandBufferType == LOGIC_AND_CMD)
	{
		if (info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->commandBufferType == LOGIC_OR_CMD)
	{
		if (!info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * replace_alias - This function replaces aliases in the tokenized string
 * @info: This is the parameter struct.
 *
 * Return: This returns 1 if replaced, 0 otherwise.
 */
int replace_alias(ShellContext *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}

/**
 * replace_vars - This function replaces variables in the tokenized string
 * @info: This is the parameter struct.
 *
 * Return: This returns 1 if replaced, 0 otherwise.
 */
int replace_vars(ShellContext *info)
{
	int i = 0;
	list_t *node;

	while (info->argv[i])
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
		{
			i++;
			continue;
		}

		if (!_strcmp(info->argv[i], "$?"))
		{
			replace_string(&(info->argv[i]),
				_strdup(convert_number(info->status, 10, 0)));
		}
		else if (!_strcmp(info->argv[i], "$$"))
		{
			replace_string(&(info->argv[i]),
				_strdup(convert_number(getpid(), 10, 0)));
		}
		else
		{
			node = node_starts_with(info->env, &info->argv[i][1], '=');
			if (node)
			{
				replace_string(&(info->argv[i]),
					_strdup(_strchr(node->str, '=') + 1));
			}
			else
			{
				replace_string(&info->argv[i], _strdup(""));
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
