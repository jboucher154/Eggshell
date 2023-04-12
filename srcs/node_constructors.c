/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_constructors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:54:23 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/12 16:31:46 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"

/*
** setup new executable command node
*/
//return pointer to the endquote
char	*find_endquote(char *begin_quote)
{
	char *temp;

	while (*temp && *temp != *begin_quote)
		temp++;
	return (temp);
}

//return pointer to the whitespace after word
char	*find_end_word(char *begin_word)
{
	char *temp;

	while (*temp && !ft_strchr(WHITESPACE, *temp))
		temp++;
	return (temp);
}

int	count_args(char *arg_start)
{
	int count;
	char *temp;

	temp = arg_start;
	count = 0;
	while (*temp != '\0' && !ft_strchr(TOKENS, *temp))
	{
		if (ft_strchr(QUOTES, *temp))
		{
			count++;
			temp += find_quote_end(temp) + 1; // I am assuming that there is whitespace on the other side of the quote, we can change it if needed
		}
		else if (ft_strchr(WHITESPACE, *temp))
		{
			while (temp && ft_strchr(WHITESPACE, *temp))
				temp++;
		}
		else
		{
			count++;
			temp = find_end_word(temp);
		}
	}
	return (count);
}

int	find_args(char *cmd_start, char *cmd_end, char ***args, char ***arg_ends)
{
	int		index;
	char	*temp;
	int		argc;
	
	temp = cmd_end + 1;
	//first need count of them to malloc the arrays
	argc = count_args(temp); //
	*args = (char **) malloc(sizeof(char *) * (argc + 1));
	if (!*args)
		return (FALSE); //need better fail system....
	*arg_ends = (char **) malloc(sizeof(char *) * (argc + 2));
	if (!*arg_ends)
		return (FALSE); //need better fail system....
	(*args)[argc] = NULL;
	(*arg_ends)[argc] = NULL;
	(*args)[0] = cmd_start;
	(*arg_ends)[0] = cmd_end;
	index = 1;
	while (*temp != '\0' && index < argc - 1 && !ft_strchr(TOKENS, *temp))
	{
		if (ft_strchr(QUOTES, *temp))
		{
			(*args)[index] = temp;
			(*arg_ends)[index] = find_quote_end(temp) + 1; //to move the end to whitespace, or does the quote need to be overwirtten?
			temp = (*arg_ends)[index];
			index++;
		}
		else if (ft_strchr(WHITESPACE, cmd_end[index]))
		{
			while (temp && ft_strchr(WHITESPACE, *temp))
				temp++;
		}
		else
		{
			(*args)[index] = temp;
			(*arg_ends)[index] = find_end_word(temp);
			temp = (*arg_ends)[index];
			index++;
		}
	}
	retun (temp - cmd_start); //return the number of chars that were parsed since start of cmd, will adjust index for input string
}

t_executable_cmd	*new_executable_cmd(char *cmd_start, int *index) //remember to change index
{
	t_executable_cmd	*new;
	char				*cmd_end;

	new = (t_executable_cmd *)malloc(sizeof(t_executable_cmd));
	if (new == NULL)
		return (NULL);
	new->type = EXECUTABLE_CMD;
	new->cmd_start = cmd_start;

	cmd_end = cmd_start;
	while (!ft_strchr(WHITESPACE, *cmd_end)) // do I need to check tokens here?
		cmd_end++;
	new->cmd_end = cmd_end; //should point to whitespace char
	(*index) += find_args(cmd_start, cmd_end, &new->args, &new->arg_ends);
	return (new);
}

/*
** setup new pipe node
*/
t_pipe	*new_pipe(t_executable_cmd *left, t_executable_cmd *right)
{
	t_pipe	*new;

	new = (t_pipe *)malloc(sizeof(t_pipe));
	if (new == NULL)
		return (NULL);
	new->type = PIPE;
	new->left = (t_cmd *) left;
	new->right = (t_cmd *) right;
	return (new);
}

/*
** setup new redirection node
*/
int	find_filename(char *token_start, char **fstart, char **fend, char token_id)
{
	char	*tmp;

	if (token_id == REDIRECT_OUT_APPEND)
		tmp = token_start + 2;
	else
		tmp = token_start + 1;
	while (ft_strchr(WHITESPACE, *tmp))
		tmp++;
	*fstart = tmp;
	while (!ft_strchr(WHITESPACE, *tmp))
		tmp++;
	*fend = tmp; //should point to whitespace char
	return (*fend - *fstart); //will this be the length of the filename?
	//set fstart to first non-whitespace char after token_start
	//set fend to first whitespace char after fstart
}

t_redirection	*new_redirection(t_cmd *cmd, char *token_start, char token_id, int *index) //should the index be modified by pointer here?
{
	t_redirection	*new;


	new = (t_redirection *)malloc(sizeof(t_redirection));
	if (new == NULL)
		return (NULL);
	new->type = REDIRECTION_CMD;
	new->cmd = (t_cmd *) cmd;
	(*index) += find_filename(token_start, &new->file_start, &new->file_end, token_id);
	
	//set fstart to first non-whitespace char after token_start
	//set fend to first whitespace char after fstart

	return (new);
}

/*
** setup new logical opperator node
*/
t_logical_opperator	*new_logical_opperator(t_cmd *left, t_cmd *right, char token_id)
{
	t_logical_opperator	*new;

	new = (t_logical_opperator *)malloc(sizeof(t_logical_opperator));
	if (new == NULL)
		return (NULL);
	new->type = LOGICAL_OPERATOR;
	new->left = (t_cmd *) left;
	new->right = (t_cmd *) right;
	new->opperator = token_id;
	return (new);
}

/*
** setup new line node
*/
t_line	*new_line(t_cmd *left, t_cmd *right)
{
	t_line	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (new == NULL)
		return (NULL);
	new->type = LINE;
	new->left = (t_cmd *) left;
	new->right = (t_cmd *) right;
	return (new);
}

/*
** setup new block node
*/
t_block	*new_block(t_cmd *left, t_cmd *right, char token_id)
{
	t_block	*new;

	new = (t_block *)malloc(sizeof(t_block));
	if (new == NULL)
		return (NULL);
	new->type = BLOCK;
	new->left = (t_cmd *) left;
	new->right = (t_cmd *) right;
	new->block_id = token_id;
	return (new);
}