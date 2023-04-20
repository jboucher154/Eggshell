
#ifndef MINISHELL_H
# define MINISHELL_H
// colours
#define GREEN "\e[1;92m"
#define KNRM  "\x1B[0m"
#define RED     "\033[31m"
#define PROMPT "\e[1;92m🐣[EggShell] % \x1B[0m"


#include "libft.h"
#include "ft_hash.h"


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

//struct to send to child
typedef struct s_child
{
	int		fd_in;
	int		fd_out;
	int		fd_to_close;
	char 	*path;
	char	**args;
}	t_child;

//struct for program
typedef	struct s_eggcarton
{
	struct s_hash_table	*environment;
	struct s_hash_table *commands;
	char				*og_env;
	int					cmd_count;
	int					pipe_count;
	int					*pipes;
	int					*pids;
	struct s_child		**children;
}	t_eggcarton;


//functions for PARSER



#endif