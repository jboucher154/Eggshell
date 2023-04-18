
#ifndef MINISHELL_H
# define MINISHELL_H
// colours
#define GREEN "\e[1;92m"
#define KNRM  "\x1B[0m"
#define RED     "\033[31m"
#define PROMPT "\e[1;92m🐣[EggShell] % \x1B[0m"


#include "libft.h"


// for printf, perror
#include <stdio.h> 

//need includes for readline
#include <readline/readline.h>
#include <readline/history.h>

//for malloc, free, exit, ttyslot(System V-like systems), getenv
#include <stdlib.h>

//for write, access, read, close, fork, getcwd, chdir, stat, lstat, fstat, 
// unlink, execve, dup, dup2, pipe, isatty, ttyname, ttyslot(BSD-like systems and Linux)
// tcsetattr, tcgetattr
#include <unistd.h>

//for open,
#include <fcntl.h>

//for wait, waitpid, wait3, wait4
#include <sys/wait.h>

//for signal, sigaction, sigemptyset, sigaddset, kill
#include <signal.h>

//for opendir, stat, lstat, fstat, closedir
#include <sys/types.h>

//for readdir, opendir, closefir
#include <dirent.h>

//for stat, lstat, fstat
#include <sys/stat.h>

//for sterror
#include <string.h>

//for ioctl
#include <sys/ioctl.h>

//for tcsetattr, tcgetattr
#include <termios.h>

//for tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
#include <curses.h>

//for tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
#include <term.h>

//for error codes
#include <errno.h>



//functions for PARSER



#endif