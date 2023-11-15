#include "shell.h"

/**
 * add_node - This function adds a node to the beginning of the list.
 * @head: The head of the list.
 * @str: The str field of node.
 * @num: This is the index of the node.
 *
 * Return: It returns a pointer to the new node.
 */
list_t *add_node(list_t **head, const char *str, int num)
{
	list_t *new_head;

	if (!head)
		return (NULL);

	new_head = malloc(sizeof(list_t));
	if (!new_head)
		return (NULL);

	_memset((void *)new_head, 0, sizeof(list_t));
	new_head->num = num;

	if (str)
	{
		new_head->str = _strdup(str);
		if (!new_head->str)
		{
			free(new_head);
			return (NULL);
		}
	}

	new_head->next = *head;
	*head = new_head;
	return (new_head);
}

/**
 * add_node_end - This function adds a node to the end of the list.
 * @head: The head of the list.
 * @str: The str field of node.
 * @num: This is the index of the node.
 *
 * Return: Pointer to the new node
 */
list_t *add_node_end(list_t **head, const char *str, int num)
{
	list_t *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);

	_memset((void *)new_node, 0, sizeof(list_t));
	new_node->num = num;

	if (str)
	{
		new_node->str = _strdup(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}

	if (node)
	{
		for (; node->next; node = node->next)
			;
		node->next = new_node;
	}
	else
		*head = new_node;

	return (new_node);
}

/**
 * print_list_str - It displays the list.
 * @h: The pointer to the head of the list.
 *
 * Return: The number of nodes displayed.
 */
size_t print_list_str(const list_t *h)
{
	size_t i;

	for (i = 0; h; i++, h = h->next)
	{
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
	}

	return (i);
}

/**
 * delete_node_at_index - It deletes a node at a given index.
 * @head: The head of the list.
 * @index: The index of the node to be deleted.
 *
 * Return: 1 on success, 0 otherwise
 */
int delete_node_at_index(list_t **head, unsigned int index)
{
	list_t *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}

	node = *head;
	for (; node; i++, node = node->next)
	{
		if (i == index)
		{
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}

		prev_node = node;
	}

	return (0);
}

/**
 * free_list - This function frees the list.
 * @head_ptr: The pointer to the head of the list.
 *
 * Return: It returns void.
 */
void free_list(list_t **head_ptr)
{
	list_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;

	head = *head_ptr;

	for (node = head; node; node = next_node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
	}

	*head_ptr = NULL;
}

