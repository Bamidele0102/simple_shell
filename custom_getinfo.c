#include "shell.h"

/**
 * clear_context - This is used to initialize the ShellContext struct.
 * @context: It is the address of the ShellContext struct.
 */
void clear_context(ShellContext *context)
{
	context->arg = NULL;
	context->argv = NULL;
	context->path = NULL;
	context->argc = 0;
}

/**
 * set_context - This is used to initialize the ShellContext struct.
 * @context: It is the address of the ShellContext struct.
 * @av: The command line arguments.
 */
void set_context(ShellContext *context, char **av)
{
	context->fileName = av[0];

	if (context->arg)
	{
		context->argv = strtow(context->arg, " \t");
		if (context->argv == NULL)
		{
			context->argv = malloc(sizeof(char *) * 2);
			if (context->argv != NULL)
			{
				context->argv[0] = _strdup(context->arg);
				context->argv[1] = NULL;
			}
		}

		if (context->argv != NULL)
		{
			int i;

			for (i = 0; context->argv[i]; i++)
				;
			context->argc = i;

			replace_alias(context);
			replace_vars(context);
		}
	}
}

/**
 * free_context - Frees the memory allocated for the ShellContext struct
 * @context: The address of the ShellContext struct.
 * @all: If all is 1, it frees memory allocated for the ShellContext struct.
 */
void free_context(ShellContext *context, int all)
{
	ffree(context->argv);
	context->argv = NULL;
	context->path = NULL;

	if (all)
	{
		if (!context->commandBuffer)
			free(context->arg);
		free_list(&(context->env));
		free_list(&(context->history));
		free_list(&(context->alias));
		ffree(context->environ);
		context->environ = NULL;
		deallocate((void **)context->commandBuffer);
		if (context->input_fd > 2)
			close(context->input_fd);
		_putchar(BUFFER_FLUSH);
	}
}

