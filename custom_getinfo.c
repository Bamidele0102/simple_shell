#include "shell.h"

/**
 * clear_info - This is used to initialize the ShellContext struct.
 * @info: It is the address of the ShellContext struct.
 */
void clear_info(ShellContext *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_info - This is used to initialize the ShellContext struct.
 * @info: It is the address of the ShellContext struct.
 * @av: The command line arguments.
 */
void set_info(ShellContext *info, char **av)
{
	info->fileName = av[0];

	if (info->arg)
	{
		info->argv = strtow(info->arg, " \t");
		if (info->argv == NULL)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv != NULL)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}

		if (info->argv != NULL)
		{
			int i;

			for (i = 0; info->argv[i]; i++)
				;
			info->argc = i;

			replace_alias(info);
			replace_vars(info);
		}
	}
}

/**
 * free_info - Frees the memory allocated for the ShellContext struct fields.
 * @info: The address of the ShellContext struct.
 * @all: If all is 1, it frees memory allocated for the ShellContext struct.
 */
void free_info(ShellContext *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;

	if (all)
	{
		if (!info->commandBuffer)
			free(info->arg);
		free_list(&(info->env));
		free_list(&(info->history));
		free_list(&(info->alias));
		ffree(info->environ);
		info->environ = NULL;
		bfree((void **)info->commandBuffer);
		if (info->input_fd > 2)
			close(info->input_fd);
		_putchar(BUFFER_FLUSH);
	}
}

