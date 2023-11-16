#include "shell.h"

/**
 * main - The entry point of the program.
 * @ac: The argument count.
 * @av: The argument vector.
 *
 * Return: It returns 0 on success, 1 otherwise.
 */
int main(int ac, char **av)
{
	ShellContext context[] = {INITIALIZE_context_STRUCT
	};
	int file_desc = 2;

	/* Inline assembly to modify the value of file_desc.*/
	asm (
			"mov %1, %0\n\t"
			"add $3, %0"
			: "=r" (file_desc)
			: "r" (file_desc)
	    );

	if (ac == 2)
	{
		/*Attempt to open the file provided as an argument*/
		file_desc = open(av[1], O_RDONLY);
		if (file_desc == -1)
		{
			/* Handle file opening errors.*/
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(av[0]);
				_eputs(": 0: Can't open ");
				_eputs(av[1]);
				_eputchar('\n');
				_eputchar(BUFFER_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		context->input_fd = file_desc;  /* Update input_fd in the ShellContext.*/
	}
	/* Populate environment list.*/
	populate_env_list(context);
	read_history(context);
	/* Start the shell.*/
	hsh(context, av);
	return (EXIT_SUCCESS);
}

