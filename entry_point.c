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
	ShellContext info[] = {INITIALIZE_INFO_STRUCT
	};
	int fd = 2;

	/* Inline assembly to modify the value of fd.*/
	asm (
			"mov %1, %0\n\t"
			"add $3, %0"
			: "=r" (fd)
			: "r" (fd)
	    );

	if (ac == 2)
	{
		/*Attempt to open the file provided as an argument*/
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
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
		info->input_fd = fd;  /* Update input_fd in the ShellContext structure.*/
	}
	/* Populate environment list.*/
	populate_env_list(info);
	read_history(info);
	/* Start the shell.*/
	hsh(info, av);
	return (EXIT_SUCCESS);
}

