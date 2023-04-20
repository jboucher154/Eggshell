
#ifndef MINISHELL_H
# define MINISHELL_H
// colours
#define GREEN "\e[1;92m"
#define KNRM  "\x1B[0m"
#define RED     "\033[31m"
#define PROMPT "\e[1;92m🐣[EggShell] % \x1B[0m"


# include "libft.h"
# include "ft_hash.h"
# include "ft_ast.h"

// for printf, perror
# include <stdio.h> 

//need includes for readline
# include <readline/readline.h>
# include <readline/history.h>

//for malloc, free, exit, ttyslot(System V-like systems), getenv
# include <stdlib.h>

//for write, access, read, close, fork, getcwd, chdir, stat, lstat, fstat, 
// unlink, execve, dup, dup2, pipe, isatty, ttyname, ttyslot(BSD-like systems and Linux)
// tcsetattr, tcgetattr
# include <unistd.h>

//for open,
# include <fcntl.h>

//for wait, waitpid, wait3, wait4
# include <sys/wait.h>

//for signal, sigaction, sigemptyset, sigaddset, kill
# include <signal.h>

//for opendir, stat, lstat, fstat, closedir
# include <sys/types.h>

//for readdir, opendir, closefir
# include <dirent.h>

//for stat, lstat, fstat
# include <sys/stat.h>

//for sterror
# include <string.h>

//for ioctl
# include <sys/ioctl.h>

//for tcsetattr, tcgetattr
# include <termios.h>

//for tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <curses.h>

//for tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <term.h>

//for error codes
# include <errno.h>

//struct to send to child
typedef struct s_child
{
	int		redir_in;
	int		redir_out;
	int		pipe_in;
	int		pipe_out;
	int		fd_to_close;//might not be needed
	char 	*path;
	char	**args;
}	t_child;

//struct for program
typedef	struct s_eggcarton
{
	struct s_hash_table	*environment;
	struct s_hash_table *commands;
	char				**og_env;
	int					cmd_count;
	int					pipe_count;
	int					*pipes;
	int					*pids;
	struct s_child		**children;
}	t_eggcarton;


//functions for PARSER


//pathfinding.c
char	*get_path(t_eggcarton *prog_info, char *fname);
int		initalize_command_table(t_eggcarton *prog_info);
char	*find_correct_path(char *fname, char **paths);
char	**get_paths(t_eggcarton *prog_info);

//executer.c
void	executer(t_cmd *cmd, t_eggcarton *prog_info);
int		tree_iterator(t_cmd *cmd, t_eggcarton *prog_info, int *index);
void	setup_child(t_executable_cmd *cmd, t_eggcarton *prog_info, int index);
void	check_for_expansions(t_eggcarton *prog_info, char **to_check_arr);
char	*expand_env_var(t_eggcarton *prog_info, char *str, char *variable_start);
char	*quote_cut(char *to_check_str);
void	setup_pipes(t_eggcarton *prog_info, int index);
void	setup_redirection(t_redirection *redirection, t_eggcarton *prog_info, int index);
int		create_child_array(t_eggcarton *prog_info);
void	free_children(t_child **children);
t_child	*new_child(void);
int		create_pipes(t_eggcarton *prog_info);
void 	do_command(t_executable_cmd *cmd, t_eggcarton *prog_info);
void	run_system_executable(t_executable_cmd *cmd, t_eggcarton *prog_info);
void	run_builtins(t_executable_cmd *cmd, t_eggcarton *prog_info);
void	exit_child(char *error_msg, char *arg, int exit_code);


//parser.c
t_cmd	*parser(char *input_string, t_eggcarton *prog_info);

//builtins.c
void	echo_command(char **args);
void	pwd_command(void);
void	cd_command(char	**args, t_hash_table *ht_env);
void	unset_command(char **args, t_hash_table *ht_env);
void	export_command(char **args, t_hash_table *ht_env);
void	print_enviroment(t_hash_table	*ht_env);
void	clearing(void);

//print tree helpers
void	print_children(t_child **childs);
void	print_tree(t_cmd *cmd, int	depth);
void	print_array(char **array);


#endif