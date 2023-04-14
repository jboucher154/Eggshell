/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_parse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 15:05:47 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/14 17:06:25 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"

/*
** setup new executable command node
*/
//return pointer to the endquote
int	find_endquote(char *begin_quote)
{
	// char *temp;
	int	 len_quote; //inclusive of quotes

	len_quote = 1; //start at 1 as we are already at the first quote and will end on the endquote
	while (begin_quote[len_quote] && begin_quote[len_quote] != *begin_quote)
		len_quote++;
	return (len_quote);
}

//return pointer to the whitespace after word
int	find_end_word(char *begin_word)
{
	// char *temp;
	int	len_word;

	len_word = 0; //start at 0 as we are already at the first char of the word and will end on whitespace
	while (begin_word[len_word] && !ft_strchr(WHITESPACE, begin_word[len_word]))
		len_word++;
	return (len_word);
}

char	*get_arg(char **parsed_string)
{
	char	*arg;
	int		len_to_copy;

	if (ft_strchr(QUOTES, **parsed_string))
	{
		len_to_copy = find_endquote(*parsed_string) + 1;
	}
	else
	{
		len_to_copy = find_end_word(*parsed_string);
	}
	arg = ft_substr(*parsed_string, 0, len_to_copy);
	(*parsed_string) += len_to_copy + 1; //move to the whitespace
	printf("PARSED string: %s\n", *parsed_string);
	return (arg);
}

t_executable_cmd	*new_executable_cmd(void) //remember to change index
{
	t_executable_cmd	*new;
	char				*args;
	int					index;

	printf("IN NEW EXECUTABLE\n");
	new = (t_executable_cmd *) malloc(sizeof(t_executable_cmd));
	if (!new)
		return (NULL);
	printf("IN NEW EXECUTABLE: after malloc of struct\n");
	new->type = EXECUTABLE_CMD;
	new->cmd_path = NULL;
	new->args = (char **) malloc(sizeof(char *) * 11);
	if (!new->args)
	{
		free(new);
		return (NULL);
	}
	index = 0;
	while (index < 11)
	{
		new->args[index] = NULL;
		index++;
	}
	return (new);
}

t_cmd	*handle_exec(char **parsed_string, char *end)
{
	t_executable_cmd	*cmd;
	t_cmd				*head_cmd;
	int					arg_count;

	printf("IN HANDLE EXEC\n");
	arg_count = 0;
	cmd = new_executable_cmd(); //don't pass args to it, build it all here
	if (!cmd)
		return (NULL);
	head_cmd = (t_cmd *) cmd;
	//nextcheck for redirection
	head_cmd = handle_redirection(head_cmd, parsed_string, end);
	while (*parsed_string < end && !ft_strchr("|;", **parsed_string))
	{
		printf("PARSED string: %s\n", *parsed_string);
		while (ft_strchr(WHITESPACE, **parsed_string))
			(*parsed_string)++;
		cmd->args[arg_count] = get_arg(parsed_string);
		if (!cmd->args[arg_count])
		{
			printf("ERROR!!!!!!!!!!\n");
		}
		arg_count++; //if arg count get > 10, need to resize it!
		//check for redirections
		head_cmd = handle_redirection(head_cmd, parsed_string, end);
	}
	return (head_cmd);
}