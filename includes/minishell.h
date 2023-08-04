/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:29:27 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 15:41:59 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
// colours
# define PROMPT "\e[1;92m🐣[EggShell] % \x1B[0m"
# define UNSET	-2
# define OPEN_ERROR -1
# define CMD_ERROR 127
# define HEREDOC_TEMP ".heredoc"
// # define 
//tokens
# define TOKENS "|<>"
# define WHITESPACE " \t\n\r\a"
# define QUOTES "'\""
//boolean
# define TRUE 1
# define FALSE 0
# define ON 1
# define OFF 0

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
// unlink, execve, dup, dup2, pipe, isatty, ttyname, 
// ttyslot(BSD-like systems and Linux), tcsetattr, tcgetattr
# include <unistd.h>

//for open,
# include <fcntl.h>

//for wait, waitpid, wait3, wait4
# include <sys/wait.h>

//for signal, sigaction, sigemptyset, sigaddset, kill
# include <signal.h>

//for sterror
# include <string.h>

//for tcsetattr, tcgetattr
# include <termios.h>

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
typedef struct s_exec
{
	int		type;
	char	*cmd_path;
	char	**args;
	int		arg_count;
}	t_exec;

// pipe = '|'
typedef struct s_pipe
{
	int				type;
	struct s_cmd	*left;
	struct s_cmd	*right;
}	t_pipe;

// redirectors = '<' '>' '>>'
typedef struct s_redirection
{
	int				type;
	struct s_cmd	*cmd;
	char			*filename;
	int				token_id;
	int				fd;
	int				expand_variable;
	int				from_fd;
}	t_redirection;

//struct to send to child
typedef struct s_child
{
	int		command_present;
	int		pid;
	int		redir_in;
	int		redir_out;
	char	*here_doc;
	int		pipe_in;
	int		pipe_out;
	char	*path;
	char	**args;
}	t_child;

//struct for program
typedef struct s_eggcarton
{
	struct termios		*saved_term;
	struct s_hash_table	*environment;
	struct s_hash_table	*command_table;
	char				**array_env;
	int					cmd_count;
	int					pipe_count;
	int					*pipes;
	struct s_child		**children;
	int					should_execute;
}	t_eggcarton;

//struct for quote tracking
typedef struct s_quote_tracker
{
	char	quote_type;
	int		in_quote;
	int		quote_index;
}	t_quote_tracker;

//struct for executable parse info
typedef struct s_exec_parse_info
{
	int						current_size;
	struct s_exec			*cmd;
	struct s_cmd			*head_cmd;
}	t_exec_parse_info;

//from readline library
void	rl_replace_line(const char *text, int clear_undo);

//validate_syntax.c
int		validate_syntax(char *str, t_eggcarton *prog_info);
int		validate_redirect(char **token_start);
int		validate_redirect_out_append(char **token);

//parse.c
t_cmd	*handle_exec(char **parsed_string, char *end, t_eggcarton *prog_info);

//tokenize.c
char	move_to_token(char **parse_string, char *end);
char	identify_token(char *token_start);
int		peek_next_token(char *current_index, char *look_for);

//node_constructors.c
t_cmd	*new_pipe(t_cmd *left, t_cmd *right);

//redirection_parse.c
t_cmd	*handle_redirection(t_cmd *cmd, \
		char **parsed_string, char *end, t_eggcarton *prog_info);
t_cmd	*new_redirection(t_cmd *cmd, char **file_start, \
		char token_id, t_eggcarton *prog_info);

//executable_parse.c
t_exec	*new_executable_cmd(void);
char	*get_arg(char **parsed_string, t_eggcarton *prog_info, int expand);

//pathfinding.c
char	*get_path(t_eggcarton *prog_info, char *fname);
int		initalize_command_table(t_eggcarton *prog_info);

//executer.c
void	executer(t_cmd **cmd, t_eggcarton *prog_info);
void	run_builtins(t_child *cmd, t_eggcarton *prog_info);
void	pipe_child(t_eggcarton *prog_info, int inde);
void	process_redirections(t_redirection *redir, t_eggcarton *prog_info, \
		int index);

//command_tree.c
int		create_pipes(t_eggcarton *prog_info);
int		tree_iterator(t_cmd *cmd, t_eggcarton *prog_info, int *index);
void	setup_pipes(t_eggcarton *prog_info, int index);

//reset.c
void	clean_str_array(char **array);
void	close_pipes(int *pipes, int pipe_count);
void	clean_tree(t_cmd *cmd);
void	reset_program(t_eggcarton *prog_info, t_cmd **cmd);

//parser.c
t_cmd	*parser(char *input_string, t_eggcarton *prog_info);

//builtins
int		echo_command(char **args);
int		pwd_command(void);
int		cd_command(char	**args, t_eggcarton *prog_info);
int		unset_command(char **args, t_eggcarton *prog_info);
int		export_command(char **args, t_eggcarton *prog_info);
int		print_enviroment(t_hash_table	*ht_env);
void	exit_command(t_eggcarton *prog_info, t_child *cmd);
void	heredoc_builtin(t_eggcarton *prog_info, t_redirection *redirection, \
		int index);
int		is_valid_var_name(char *key);

//exit.c
void	exit_command(t_eggcarton *prog_info, t_child *cmd);
void	clean_and_restore(t_eggcarton *prog_info);

//signal.c
void	initialize_signals(void);
void	initialize_child_signals(void);
void	initialize_heredoc_signals(void);

//eggshell.c
int		eggshell(t_eggcarton *prog_info);

//children.c
int		create_child_array(t_eggcarton *prog_info);
void	free_children(t_child **children);

//expansions.c
char	*check_for_expansions(t_eggcarton *prog_info, char *str_to_assess);
char	*expand_env_var(t_eggcarton *prog_info, char *str, \
		char *variable_start, int *index);

//utility.c
void	move_pointer_past_ws(char **str_to_move);
void	move_pointer_backwards_ws(char **str_to_move, char *dont_go_past);
int		print_error(char *error_msg);
void	close_redirections(int fd_in, int fd_out);
void	echoctl_switch(int toggle);
int		print_errno_error(void);
int		print_blame_error(char *program, char *error_msg, char *to_blame);
void	update_environment_array(t_eggcarton *prog_info);
char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3);
void	ft_free_two(char *to_free1, char *to_free2);
int		print_errno_blame(char *program, char *to_blame);
char	*get_heredoc_name(int index);

//initialize.c
int		initialize_eggcarton(t_eggcarton *prog, char **env, \
		struct termios *term);
#endif