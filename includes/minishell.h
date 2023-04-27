
#ifndef MINISHELL_H
# define MINISHELL_H
// colours
# define GREEN "\e[1;92m"
# define KNRM  "\x1B[0m"
# define RED     "\033[31m"
# define PROMPT "\e[1;92m🐣[EggShell] % \x1B[0m"
# define UNSET	-2
# define OPEN_ERROR -1
# define CMD_ERROR 127
// # define 
//tokens
#define TOKENS "|<>&;" //"|&<>();"
#define WHITESPACE " \t\n\r\a"
#define QUOTES "'\""
//boolean
#define TRUE 1
#define FALSE 0

# include "libft.h"
# include "ft_hash.h"

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

enum e_tokens
{
	ALPHA = 'a',
	PIPE = '|',
	REDIRECT_IN = '<',
	REDIRECT_OUT = '>',
	REDIRECT_OUT_APPEND = '+',
	REDIRECT_HERE = 'h'
};

//types of commands
enum e_type
{
	EXECUTABLE_CMD,
	PIPE_CMD,
	REDIRECTION_CMD,
	LINE,
};

//this is the base struct for all the other structs
typedef struct s_cmd
{
	int	type;
}	t_cmd;

//this is the struct for the executable command
//has pointers to beginning and end of the command
//has a pointer to the arguments for the command
typedef struct s_executable_cmd
{
	int		type;
	char	*cmd_path;
	char	**args;
}	t_executable_cmd;

// pipe = '|'
typedef struct s_pipe
{
	int	type;
	struct s_cmd	*left;
	struct s_cmd	*right;
}	t_pipe;

// redirectors = '<' '>' '>>'
typedef struct s_redirection
{
	int	type;
	struct s_cmd	*cmd;
	char			*filename;
	int				token_id;
	int				fd;
	int				from_fd;
}	t_redirection;


//struct to send to child
typedef struct s_child
{
	int		redir_in;
	int		redir_out;
	int		pipe_in;
	int		pipe_out;
	char 	*path;
	char	**args;
}	t_child;

//struct for program
typedef	struct s_eggcarton
{
	struct s_hash_table	*environment;
	struct s_hash_table *command_table;
	char				**og_env;
	int					cmd_count;
	int					pipe_count;
	int					*pipes;
	int					*pids;
	struct s_child		**children;
}	t_eggcarton;


//functions for PARSER
////////////////////////// FROM AST_H
//validate_syntax.c
int	validate_syntax(char *str);
int	validate_pipe(char *token, char *str);
int	validate_redirect_in(char *token_start);
int	validate_redirect_out(char *token_start);
int	validate_redirect_out_append(char **token_start);
int  validate_quotes(char **token);
// int	validate_or(char *token_start);
// int	validate_and(char *token_start);
// int	validate_open_parenthesis(char *token_start);
// int	validate_close_parenthesis(char *token_start);

//parse.c
// t_cmd	*parser(char *input_string, t_eggcarton *prog_info);
// t_cmd	*parse_line(char **parse_string, char *end);
// t_cmd	*handle_block(char **parse_string, char *end);
t_cmd	*handle_redirection(t_cmd *cmd, char **parse_string, char *end);
t_cmd	*handle_exec(char **parsed_string, char *end, int *cmd_count, t_eggcarton *prog_info);
t_cmd	*handle_pipe(char **parsed_string, char *end, int *cmd_count, int *pipe_count, t_eggcarton *prog_info);

//tokenize.c
char	move_to_token(char **parse_string, char *end);
char	identify_token(char *token_start);
int		peek_next_token(char *current_index, char *look_for);

//node_constructors.c
t_cmd	*new_line(t_cmd *left, t_cmd *right);
t_cmd	*new_pipe(t_cmd *left, t_cmd *right);

//redirection_parse.c
t_cmd	*handle_redirection(t_cmd *cmd, char **parsed_string, char *end);
t_cmd	*new_redirection(t_cmd *cmd, char **file_start, char token_id);
char	*find_filename(char *file_start);//for redirection

//executable_parse.c
t_executable_cmd	*new_executable_cmd(void);
char				*find_args(char *cmd_start, char ***args); //for the executable command
int					count_args(char *arg_start); //for the executable command
// int				skip_redirection(char *redir_start);//for the executable command
int					find_end_word(char *begin_word);//for the executable command
int					find_endquote(char *begin_quote);//for the executable command

//PRINT_TREE
void	print_tree(t_cmd *cmd, int	depth);

///////////////////////////////

//pathfinding.c
char	*get_path(t_eggcarton *prog_info, char *fname);
int		initalize_command_table(t_eggcarton *prog_info);
char	*find_correct_path(char *fname, char **paths);
char	**get_paths(t_eggcarton *prog_info);

//executer.c
void	executer(t_cmd *cmd, t_eggcarton *prog_info);
void 	do_commands(t_eggcarton *prog_info);
void	run_system_executable(t_executable_cmd *cmd, t_eggcarton *prog_info);
void	run_builtins(t_child *cmd, t_eggcarton *prog_info);
void	exit_child(char *error_msg, char *arg, int exit_code);

//command_tree.c
int		create_pipes(t_eggcarton *prog_info);
int		tree_iterator(t_cmd *cmd, t_eggcarton *prog_info, int *index);
void	setup_child(t_executable_cmd *cmd, t_eggcarton *prog_info, int index);
void	setup_pipes(t_eggcarton *prog_info, int index);
void	setup_redirection(t_redirection *redirection, t_eggcarton *prog_info, int index);

//reset.c
void 	clean_str_array(char **array);
void 	close_pipes(int *pipes, int pipe_count);
void	clean_tree(t_cmd *cmd);
void 	reset_program(t_eggcarton *prog_info, t_cmd **cmd);

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

//validate_syntax.c


//children.c
int		create_child_array(t_eggcarton *prog_info);
void	free_children(t_child **children);
t_child	*new_child(void);

//expansions.c
char	*check_for_expansions(t_eggcarton *prog_info, char *str_to_assess);
char	*expand_env_var(t_eggcarton *prog_info, char *str, char *variable_start);
void	insert_new_value(char *str, char *new_str, char *variable, char *value);
char	*quote_cut(char *to_check_str);

//untility.c
void	move_pointer_past_ws(char **str_to_move);
int		print_error(char *error_msg);
void	close_redirections(int fd_in, int fd_out);


#endif