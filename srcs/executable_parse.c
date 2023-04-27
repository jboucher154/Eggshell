/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_parse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 15:05:47 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/27 10:51:16 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** setup new executable command node
*/
int	find_endquote(char *begin_quote)
{

	int	 len_quote;

	len_quote = 1;
	while (begin_quote[len_quote] && begin_quote[len_quote] != *begin_quote)
		len_quote++;
	return (len_quote);
}

int	find_end_word(char *begin_word)
{
	int	len_word;

	len_word = 0;
	while (begin_word[len_word] && !ft_strchr(WHITESPACE, begin_word[len_word]) && !ft_strchr(TOKENS, begin_word[len_word]))
		len_word++;
	return (len_word);
}

char *remove_quotes(char *expanded_string)
{
	char	*quoteless;
	int		index;
	char	quote_type;
	int		q_index;
	int		in_quote;
	
	index = 0;
	quoteless = ft_calloc((ft_strlen(expanded_string) + 1), sizeof(char));
	quote_type = UNSET;
	q_index = 0;
	in_quote = FALSE;
	while (expanded_string[index])
	{
		if (expanded_string[index] && in_quote == FALSE && ft_strchr(QUOTES, expanded_string[index])) //not in quote and finding a quote 
		{
			printf("QUOTE SET TO TRUE: %s, current char assessed: %c\n", quoteless, expanded_string[index]);
			in_quote = TRUE;
			quote_type = expanded_string[index];
		}
		else if (expanded_string[index] && in_quote == TRUE && ft_strchr(&quote_type, expanded_string[index]) == NULL) //in a quote, not at the end of the quote
		{
			quoteless[q_index] = expanded_string[index];
			q_index++;
		}
		else if (in_quote == TRUE && ft_strchr(&quote_type, expanded_string[index]))//in a quote, found the end of it
		{
			printf("QUOTE SET TO FALSE: %s, current char assessed: %c\n", quoteless, expanded_string[index]);
			in_quote = FALSE;
			quote_type = UNSET;
		} 
		else //not in a quote, and we don't have a quote
		{
			quoteless[q_index] = expanded_string[index];
			q_index++;
		}
		index++;
	}
	// quoteless[q_index] = '\0';
	return (quoteless);
}

char *find_end(char **parsed_string)
{
	int 	in_quote;//
	char	*end;
	char	quote_type;

	in_quote = FALSE;
	end = *parsed_string;
	quote_type = UNSET;
	while (end != NULL)
	{
		if(end == NULL || (ft_strchr(WHITESPACE, *end) && in_quote == FALSE))
			return(end);
		else if (ft_strchr(QUOTES, *end) && in_quote == FALSE)
		{
			in_quote = TRUE;
			quote_type = *end;
		}
		else if (ft_strchr(&quote_type, *end) && in_quote == TRUE)
		{
			in_quote = FALSE;
		}
		end++;
	}
	return (end);
}	

//returns allocated char * that is stored in arg array
char	*get_arg(char **parsed_string, t_eggcarton *prog_info) //points to the first char of the arg
{
	char	*arg;
	char 	*end;
	
	if (!*parsed_string)
		return (NULL);
	end = find_end(parsed_string);
	arg = ft_substr(*parsed_string, 0, end - *parsed_string);
	arg = check_for_expansions(prog_info, arg);
	arg = remove_quotes(arg);
	(*parsed_string) += end - *parsed_string;
	return (arg);
}


//////////////////////////////////////////////////////////////////////////////////////////
//remember to change index
t_executable_cmd	*new_executable_cmd(void) 
{
	t_executable_cmd	*new;
	int					index;

	new = (t_executable_cmd *) malloc(sizeof(t_executable_cmd));
	if (!new)
		return (NULL);
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

char	**resize_array(char **array, int *size)
{
	int		index;
	char	**new;

	(*size) = (*size) * 2;
	new = (char **) malloc(sizeof(char *) * ((*size) + 1));
	if (!new)
		return (NULL);
	index = 0;
	while (index < *size )
	{
		if (array[index] != NULL)
			new[index] = array[index];
		else
			new[index] = NULL;
		index++;
	}
	return (new);
}

t_cmd	*handle_exec(char **parsed_string, char *end, int *cmd_count, t_eggcarton *prog_info)
{
	t_executable_cmd	*cmd;
	t_cmd				*head_cmd;
	int					arg_count;
	int					current_size;

	arg_count = 0;
	current_size = 10;
	cmd = new_executable_cmd();
	if (!cmd)
		return (NULL);
	head_cmd = (t_cmd *) cmd;
	move_pointer_past_ws(parsed_string);
	if (**parsed_string == '<' || **parsed_string == '>')
		head_cmd = handle_redirection(head_cmd, parsed_string, end);//
	while (*parsed_string < end && !ft_strchr("|", **parsed_string))
	{
		cmd->args[arg_count] = get_arg(parsed_string, prog_info);
		if (!cmd->args[arg_count])
		{
			printf("ERROR!!!!!!!!!!\n");//
		}
		arg_count++;
		if (arg_count == current_size)
			cmd->args = resize_array(cmd->args, &current_size);
		if (!**parsed_string)
			break ;
		move_pointer_past_ws(parsed_string);
		if (**parsed_string == '<' || **parsed_string == '>')
			head_cmd = handle_redirection(head_cmd, parsed_string, end);
	}
	(*cmd_count)++;
	return (head_cmd);
}