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
