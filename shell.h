#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

/* The read/write buffers */
#define READ_THEBUFF_SIZE 1024
#define WRITE_THEBUFF_SIZE 1024
#define BUFFER_FLUSH -1

/* The command chaining types */
#define NORMAL_CMD	0
#define LOGIC_OR_CMD	1
#define LOGIC_AND_CMD	2
#define CHAINING_CMD	3

/* To convert_number() */
#define TO_LOWERCASE 1
#define TO_UNSIGNED 2

/* 1 getline() is used */
#define ENABLE_CUSTOM_GETLINE 0
#define ENABLE_CUSTOM_STRTOK 0

#define FILE_HISTORY	".simple_shell_history"
#define MAXIMUM_HISTORY	4096

extern char **environ;


/**
 * struct liststr - The list structure
 * @num: The number of the node
 * @str: The string
 * @next: This is the next node in the list
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - This contains the info for the pass command
 *		and allows uniform prototypes for function pointer struct
 *@arg: This is the string generated from getline containing arguements
 *@argv: This is an array of strings generated from arg
 *@path: The string path for the current command
 *@argc: An argument count
 *@lineCount: This is the error line count
 *@errorNumber: This is the error code for exit()s
 *@lineCountFlag: The line count flag
 *@fileName: This is the filename for the current program
 *@env: This is the environment list
 *@environ: The custom modified environment list
 *@history: Tis is the history list
 *@alias: The alias list
 *@env_changed: This is the environment changed flag
 *@status: This is the return status of the last exec'd command
 *@commandBuffer: The address of pointer to commandBuffer, on if chaining
 *@commandBufferType: CMD_type ||, &&, ;
 *@input_fd: The file descriptor from which to get line input
 *@historyCount: The history count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int lineCount;
	int errorNumber;
	int lineCountFlag;
	char *fileName;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **commandBuffer;
	int commandBufferType;
	int input_fd;
	int historyCount;
} ShellContext;

#define INITIALIZE_INFO_STRUCT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 * struct BuiltinEntry - Represents an entry in the built-in commands table
 * @type: The type of built-in command
 * @function: The function pointer to the built-in command handler
 */
typedef struct BuiltinEntry
{
	char *type;
	int (*function)(ShellContext *);
} BuiltinEntry;


/* custom_errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* our_shell_loop.c */
int hsh(ShellContext *, char **);
int find_builtin(ShellContext *);
void find_cmd(ShellContext *);
void fork_cmd(ShellContext *);

/* custom_getline.c */
ssize_t get_input(ShellContext *);
void sigint_handler(int signo);
int _getline(ShellContext *, char **, size_t *);

/* custom_errors1.c */
int _erratoi(char *);
void print_error(ShellContext *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* our_parser.c */
int is_cmd(ShellContext *, char *);
char *dup_chars(char *, int, int);
char *find_path(ShellContext *, char *, char *);

/* custom_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* _atoi.c */
int interactive(ShellContext *info);
int is_delim(char c, const char *delim);
int isalpha(int c);
int atoi(const char *str);

/* custom_variables.c */
int is_chain(ShellContext *, char *, size_t *);
void eval_chain(ShellContext *, char *, size_t *, size_t, size_t);
int replace_alias(ShellContext *);
int replace_vars(ShellContext *);
int replace_string(char **, char *);

/* custom_getinfo.c */
void clear_info(ShellContext *);
void set_info(ShellContext *, char **);
void free_info(ShellContext *, int);

/* custom_environ.c */
char *_getenv(ShellContext *, const char *);
int _myenv(ShellContext *);
int _mysetenv(ShellContext *);
int _myunsetenv(ShellContext *);
int populate_env_list(ShellContext *);

/* custom_builtin.c */
int _myexit(ShellContext *);
int _mycd(ShellContext *);
int _myhelp(ShellContext *);

/* custom_builtin1.c */
int _myhistory(ShellContext *);
int _myalias(ShellContext *);

/* custom_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);


#endif /* SHELL_H */
