#include "shell.h"

/**
 * list_len - This determines the size of the list.
 * @h: The pointer to the first node.
 *
 * Return: The size of the list.
 */
size_t list_len(const list_t *h)
{
	size_t i = 0;

	for (; h; h = h->next, i++)
		;

	return (i);
}

/**
 * list_to_strings - Converts a list to an array of strings.
 * @head: The pointer to the first node.
 *
 * Return: This returns an array of strings.
 */
char **list_to_strings(list_t *head)
{
	list_t *node = head;
	size_t i = list_len(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);

	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);

	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(_strlen(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = _strcpy(str, node->str);
		strs[i] = str;
	}

	strs[i] = NULL;
	return (strs);
}

/**
 * print_list - It displays all elements of the list.
 * @h: The pointer to the first node.
 *
 * Return: It returns the number of elements in the list.
 */
size_t print_list(const list_t *h)
{
	size_t i = 0;

	for (; h; h = h->next, i++)
	{
		_puts(convert_number(h->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
	}

	return (i);
}

/**
 * node_starts_with - This function determines if a node starts with a prefix.
 * @node: The node to check.
 * @prefix: The prefix to match.
 * @c: The character to match.
 *
 * Return: 1 if the node starts with the prefix.
 */
list_t *node_starts_with(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	for (; node; node = node->next)
	{
		p = starts_with(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
	}

	return (NULL);
}

/**
 * get_node_index - This function determines the index of a node.
 * @head: Pointer to the head of the list.
 * @node: It is the pointer to the node.
 *
 * Return: It returns the index of the node, or -1 if the node is not found.
 */
ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t i = 0;

	for (; head; head = head->next, i++)
	{
		if (head == node)
			return (i);
	}

	return (-1);
}

