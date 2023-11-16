#include "shell.h"

/**
 * get_environ - Get the environment variables.
 * @context: The context struct.
 *
 * Return: Always 0
 */
char **get_environ(ShellContext *context)
{
	if (!context->environ || context->env_changed)
	{
		context->environ = list_to_strings(context->env);
		context->env_changed = 0;
	}

	return (context->environ);
}

/**
 * _unsetenv - It deletes an environment variable.
 * @context: The context struct.
 * @var: The var to be deleted.
 * Return: 1 on delete, 0 otherwise
 */
int _unsetenv(ShellContext *context, char *var)
{
	list_node *curr_node = context->env;
	size_t index = 0;
	char *matchPointer;

	if (!curr_node || !var)
		return (0);

	for (; curr_node; curr_node = curr_node->next, index++)
	{
		matchPointer = starts_with(curr_node->str, var);
		if (matchPointer && *matchPointer == '=')
		{
			context->env_changed = delete_node_at_index(&(context->env), index);
			index = 0;
			curr_node = context->env;
			return (1);  /* Variable successfully deleted*/
		}
	}

	/* Variable not found */
	fprintf(stderr, "unsetenv: %s: Environment variable not found\n", var);
	return (0);
}

/**
 * _setenv - It sets an environment variable.
 * @context: The context struct.
 * @var: The var to be set.
 * @value: The value of the var.
 * Return: Always 0.
 */
int _setenv(ShellContext *context, char *var, char *value)
{
	char *newEnvVar = NULL;
	list_node *curr_node;
	char *matchPointer;

	if (!var || !value)
		return (0);

	newEnvVar = malloc(_strlen(var) + _strlen(value) + 2);
	if (!newEnvVar)
	{
		fprintf(stderr, "setenv: Memory allocation failed\n");
		return (1);  /* Memory allocation failed*/
	}

	_strcpy(newEnvVar, var);
	_strcat(newEnvVar, "=");
	_strcat(newEnvVar, value);

	for (curr_node = context->env; curr_node; curr_node = curr_node->next)
	{
		matchPointer = starts_with(curr_node->str, var);
		if (matchPointer && *matchPointer == '=')
		{
			free(curr_node->str);
			curr_node->str = newEnvVar;
			context->env_changed = 1;
			return (0);  /* Variable successfully set*/
		}
	}

	add_node_end(&(context->env), newEnvVar, 0);
	free(newEnvVar);
	context->env_changed = 1;
	return (0);
}
