#include "shell.h"

/**
 * bfree - Frees a pointer and NULLs the address.
 * @ptr: Address of the pointer to free.
 *
 * Return: 1 if freed, otherwise 0.
 */
int bfree(void **ptr)
{
	/* Check if the pointer and its content exist */
	if (ptr && *ptr)
	{
		/* Free the memory */
		free(*ptr);
		/* Set the pointer to NULL to avoid dangling references */
		*ptr = NULL;
		/* Return 1 to indicate successful freeing */
		return (1);
	}
	/* Return 0 if the pointer or its content is already NULL */
	return (0);
}
