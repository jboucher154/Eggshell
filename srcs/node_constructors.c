/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_constructors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:54:23 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/14 15:39:58 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"


/*
** setup new pipe node
*/
t_cmd	*new_pipe(t_cmd *left, t_cmd *right)
{
	t_pipe	*new;

	new = (t_pipe *)malloc(sizeof(t_pipe));
	if (new == NULL)
		return (NULL);
	new->type = PIPE;
	new->left = (t_cmd *) left;
	new->right = (t_cmd *) right;
	return ((t_cmd *) new);
}

/*
** setup new line node
*/
t_cmd	*new_line(t_cmd *left, t_cmd *right)
{
	t_line	*new;

	new = (t_line *)malloc(sizeof(t_line));
	if (new == NULL)
		return (NULL);
	new->type = LINE;
	new->left = (t_cmd *) left;
	new->right = (t_cmd *) right;
	return ((t_cmd *) new);
}



// /*
// ** setup new executable command node
// */
// //return pointer to the endquote
// int	find_endquote(char *begin_quote)
// {
// 	// char *temp;
// 	int	 len_quote; //inclusive of quotes

// 	len_quote = 1; //start at 1 as we are already at the first quote and will end on the endquote
// 	while (begin_quote[len_quote] && begin_quote[len_quote] != *begin_quote)
// 		len_quote++;
// 	return (len_quote);
// }

// //return pointer to the whitespace after word
// int	find_end_word(char *begin_word)
// {
// 	// char *temp;
// 	int	len_word;

// 	len_word = 0; //start at 0 as we are already at the first char of the word and will end on whitespace
// 	while (begin_word[len_word] && !ft_strchr(WHITESPACE, begin_word[len_word]))
// 		len_word++;
// 	return (len_word);
// }

// int	skip_redirection(char *redir_start)
// {
// 	// char	*temp;
// 	int		len_redir;

// 	len_redir = 0;//start at 0 as will end on whitespace after filename
// 	while (redir_start[len_redir] && (redir_start[len_redir] == '<' || redir_start[len_redir] == '>')) //skip redirection chars
// 		len_redir++;
// 	while (redir_start[len_redir] && ft_strchr(WHITESPACE, redir_start[len_redir])) //skip whitespace before filename
// 		len_redir++;
// 	while (redir_start[len_redir] && !ft_strchr(WHITESPACE, redir_start[len_redir])) //skip filename
// 		len_redir++;
// 	return (len_redir);
// }

// int	count_args(char *arg_start)
// {
// 	int count;
// 	char *temp;

// 	temp = arg_start;
// 	count = 0;
// 	while (*temp != '\0' && !ft_strchr(TOKENS, *temp))
// 	{
// 		if (ft_strchr(QUOTES, *temp))
// 		{
// 			count++;
// 			temp += find_quote_end(temp); // I am assuming that there is whitespace on the other side of the quote, we can change it if needed
// 		}
// 		else if (ft_strchr(WHITESPACE, *temp))
// 		{
// 			while (temp && ft_strchr(WHITESPACE, *temp))
// 				temp++;
// 		}
// 		else if (ft_strchr("<>", *temp)) //skip redirection in middle of args
// 		{
// 			temp += skip_redirection(temp);
// 		}
// 		else
// 		{
// 			count++;
// 			temp += find_end_word(temp);
// 		}
// 	}
// 	return (count);
// }

// //need to skip entry that is realted to a redirection
// char	*find_args(char *cmd_start, char ***args)
// {
// 	int		index;
// 	char	*temp;
// 	int		argc;
// 	int		len_to_copy;
	
// 	len_to_copy = 0;
// 	temp = cmd_start;
// 	argc = count_args(temp); //first need count of them to malloc the arrays
// 	*args = (char **) malloc(sizeof(char *) * (argc + 1));
// 	if (!*args)
// 		return (FALSE); //need better fail system....
// 	(*args)[argc] = NULL;
// 	index = 0;
// 	while (*temp != '\0' && index < argc)
// 	{
// 		if (ft_strchr("<>", *temp)) //skip redirection in middle of args
// 			temp = skip_redirection(temp);
// 		else if (ft_strchr(TOKENS, *temp))
// 			break ;
// 		else if (ft_strchr(WHITESPACE, *temp))
// 		{
// 			while (temp && ft_strchr(WHITESPACE, *temp))
// 				temp++;
// 		}
// 		else 
// 		{
// 			if (ft_strchr(QUOTES, *temp))
// 			{
// 				len_to_copy = find_endquote(temp); //does not include whitespace after quote
// 				(*args)[index] = ft_substr(temp, 0, len_to_copy);//should copy including quotes so we can eval them later for expansions
// 			}
// 			else
// 			{
// 				len_to_copy = find_end_word(temp) - 1; //exculde the WHITESPACE
// 				(*args)[index] = ft_substr(temp, 0, len_to_copy);
// 			}
// 			if (!(*args)[index])
// 				break ; //need better fail system....
// 			index++;
// 			temp += len_to_copy + 1; // advance one past the copied to assess the new entry
// 		}
// 	}
// 	retun (temp); //return the number of chars that were parsed since start of cmd, will adjust index for input string
// }

// t_cmd	*new_executable_cmd(void) //remember to change index
// {
// 	t_executable_cmd	*new;
// 	char				*args;

// 	new = (t_executable_cmd *) malloc(sizeof(t_executable_cmd));
// 	if (new == NULL)
// 		return (NULL);
// 	new->type = EXECUTABLE_CMD;
// 	new->cmd_path = NULL;
// 	new->args = (char **) malloc(sizeof(char *) * 11);
// 	if (!new->args)
// 	{
// 		free(new);
// 		return (NULL);
// 	}
// 	ft_memset(new->args, 0, 11); //zero the memory???
// 	return ((t_cmd *) new);
// }

// /*
// ** setup new logical opperator node
// */
// t_logical_opperator	*new_logical_opperator(t_cmd *left, t_cmd *right, char token_id)
// {
// 	t_logical_opperator	*new;

// 	new = (t_logical_opperator *)malloc(sizeof(t_logical_opperator));
// 	if (new == NULL)
// 		return (NULL);
// 	new->type = LOGICAL_OPERATOR;
// 	new->left = (t_cmd *) left;
// 	new->right = (t_cmd *) right;
// 	new->opperator = token_id;
// 	return (new);
// }
// /*
// ** setup new block node
// */
// t_block	*new_block(t_cmd *left, t_cmd *right, char token_id)
// {
// 	t_block	*new;

// 	new = (t_block *)malloc(sizeof(t_block));
// 	if (new == NULL)
// 		return (NULL);
// 	new->type = BLOCK;
// 	new->child_node = (t_cmd *) left;
// 	new->block_id = token_id;
// 	return (new);
// }