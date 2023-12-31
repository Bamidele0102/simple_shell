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

/* To int_to_base() */
#define TO_LOWERCASE 1
#define TO_UNSIGNED 2

/* 1 getline() is used */
#define ENABLE_CUSTOM_GETLINE 0
#define ENABLE_CUSTOM_STRTOK 0

#define FILE_HISTORY	".simple_shell_history"
#define MAXIMUM_HISTORY	4096

extern char **environ;


/**
 * struct listStruct - The list structure
 * @num: The number of the node
 * @str: The string
 * @next: This is the next node in the list
 */
typedef struct listStruct
{
	int num;
	char *str;
	struct listStruct *next;
} list_node;

/**
 *struct passcontext - This contains the context for the pass command
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
typedef struct passcontext
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int lineCount;
	int errorNumber;
	int lineCountFlag;
	char *fileName;
	list_node *env;
	list_node *history;
	list_node *alias;
	char **environ;
	int env_changed;
	int status;

	char **commandBuffer;
	int commandBufferType;
	int input_fd;
	int historyCount;
} ShellContext;

#define INITIALIZE_context_STRUCT \
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
int _putfd(char c, int file_desc);
int _putsfd(char *str, int file_desc);

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
int string_to_int(char *);
void display_error(ShellContext *, char *);
int display_decimal(int, int);
char *int_to_base(long int, int, int);
void delete_comments(char *);

/* our_parser.c */
int is_cmd(ShellContext *, char *);
char *dup_chars(char *, int, int);
char *find_path(ShellContext *, char *, char *);

/* custom_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* _atoi.c */
int interactive(ShellContext *context);
int is_delim(char c, const char *delim);
int isalpha(int c);
int atoi(const char *str);

/* custom_variables.c */
int is_chain(ShellContext *, char *, size_t *);
void eval_chain(ShellContext *, char *, size_t *, size_t, size_t);
int replace_alias(ShellContext *);
int replace_vars(ShellContext *);
int replace_string(char **, char *);

/* custom_getcontext.c */
void clear_context(ShellContext *);
void set_context(ShellContext *, char **);
void free_context(ShellContext *, int);

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

/* custom_getenv.c */
char **get_environ(ShellContext *);
int _unsetenv(ShellContext *, char *);
int _setenv(ShellContext *, char *, char *);

/* custom_history.c */
char *get_history_file(ShellContext *context);
int write_history(ShellContext *context);
int read_history(ShellContext *context);
int build_history_list(ShellContext *context, char *buffs, int linecount);
int renumber_history(ShellContext *context);

/* custom_lists.c */
list_node *add_node(list_node **, const char *, int);
list_node *add_node_end(list_node **, const char *, int);
size_t print_list_str(const list_node *);
int delete_node_at_index(list_node **, unsigned int);
void free_list(list_node **);

/* loop_hsh.c */
int loophsh(char **);

/* custom_lists1.c */
size_t list_len(const list_node *);
char **list_to_strings(list_node *);
size_t print_list(const list_node *);
list_node *node_starts_with(list_node *, char *, char);
ssize_t get_node_index(list_node *, list_node *);

/* custom_realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* custom_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* free_memory.c */
int deallocate(void **);

/* custom_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

#endif /* SHELL_H */
