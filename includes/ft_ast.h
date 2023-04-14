/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 11:42:32 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/14 15:40:19 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_AST_H
# define FT_AST_H

//tokens
#define TOKENS "|<>;" //"|&<>();"
#define WHITESPACE " \t\n\r\a"
#define QUOTES "'\""
//boolean
#define TRUE 1
#define FALSE 0

enum e_tokens
{
	ALPHA = 'a',
	PIPE = '|',
	REDIRECT_IN = '<',
	REDIRECT_OUT = '>',
	REDIRECT_OUT_APPEND = '+',
	SEMICOLON = ';',
	// OPEN_PARENTHESIS = '(',
	// CLOSE_PARENTHESIS = ')',
	// AND = '&',
	// OR = 'o',
	// BACKGROUND = 'b' //not sure if we should care about this, not going to writie it into the code
};

//types of commands
enum e_type
{
	EXECUTABLE_CMD,
	PIPE_CMD,
	REDIRECTION_CMD,
	LINE,
	// LOGICAL_OPERATOR,
	// BLOCK,
	// BACKGROUND
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
	int				direction;
	int				fd;
	int				from_fd;
}	t_redirection;


//one cmd struct links to next list
//one cmd struct links to the beginning of the execution fo the list
// list  = ';'
typedef struct s_line
{
	int				type;
	struct s_cmd	*left;
	struct s_cmd	*right;
}	t_line;

// // opperators = '&&' '||'
// typedef struct s_logical_opperator
// {
// 	int				type;
// 	struct s_cmd	*left;
// 	struct s_cmd	*right;
// 	int				opperator;
// }	t_logical_opperator;
//unsure if this is needed
//it would be for the '(' ')'
// typedef struct s_block
// {
// 	int				type;
// 	struct s_cmd	*child_node;
// 	char			block_id;// if we need to track if its open or close
// }	t_block;

// //not sure about this one, not writing any code for it
// typedef struct s_background
// {
// 	int	type;
// 	struct s_cmd	*cmd;
// } t_background;


//validate_syntax.c
int	validate_syntax(char *token_start, char token_id);
int	validate_alpha(char *token_start);
int	validate_pipe(char *token_start);
int	validate_redirect_in(char *token_start);
int	validate_redirect_out(char *token_start);
int	validate_redirect_out_append(char *token_start);
int	validate_semicolon(char *token_start);
// int	validate_or(char *token_start);
// int	validate_and(char *token_start);
// int	validate_open_parenthesis(char *token_start);
// int	validate_close_parenthesis(char *token_start);

//parse.c
t_cmd	*parser(char *input_string);
t_cmd	*parse_line(char **parse_string, char *end);
t_cmd	*handle_block(char **parse_string, char *end);
t_cmd	*handle_redirection(t_cmd *cmd, char **parse_string, char *end);
t_cmd	*handle_exec(char **parse_string, char *end);
t_cmd	*handle_pipe(char **parse_string, char *end);

//tokenize.c
char	move_to_token(char **parse_string, char *end);
char	identify_token(char *token_start);
int		peek_next_token(char *current_index, char *look_for);

//node_constructors.c
t_cmd	*new_line(t_cmd *left, t_cmd *right);
t_cmd	*new_pipe(t_cmd *left, t_cmd *right);
// t_logical_opperator	*new_logical_opperator(t_cmd *left, t_cmd *right, char token_id);
// t_block	*new_block(t_cmd *left, t_cmd *right, char token_id);

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

#endif