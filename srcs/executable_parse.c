/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_parse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 15:05:47 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/02 10:16:45 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_quote(char *quoteless, t_quote_tracker *quote_info, \
char *expanded_string, int index)
{
	if (expanded_string[index] && quote_info->in_quote == FALSE && \
		ft_strchr(QUOTES, expanded_string[index]))
	{
		quote_info->in_quote = TRUE;
		quote_info->quote_type = expanded_string[index];
	}
	else if (expanded_string[index] && quote_info->in_quote == TRUE && \
		ft_strchr(&(quote_info->quote_type), expanded_string[index]) == NULL)
	{
		quoteless[quote_info->quote_index] = expanded_string[index];
		(quote_info->quote_index)++;
	}
	else if (quote_info->in_quote == TRUE && \
		ft_strchr(&(quote_info->quote_type), expanded_string[index]))
	{
		quote_info->in_quote = FALSE;
		quote_info->quote_type = UNSET;
	}
	else
	{
		quoteless[quote_info->quote_index] = expanded_string[index];
		(quote_info->quote_index)++;
	}
}

char	*remove_quotes(char *expanded_string)
{
	char			*quoteless;
	int				index;
	t_quote_tracker	quote_info;

	index = 0;
	quoteless = ft_calloc((ft_strlen(expanded_string) + 1), sizeof(char));
	quote_info.quote_type = UNSET;
	quote_info.quote_index = 0;
	quote_info.in_quote = FALSE;
	while (expanded_string[index])
	{
		parse_quote(quoteless, &quote_info, expanded_string, index);
		index++;
	}
	return (quoteless);
}

char	*find_end(char **parsed_string)
{
	int		in_quote;
	char	*end;
	char	quote_type;

	in_quote = FALSE;
	end = *parsed_string;
	quote_type = UNSET;
	while (end != NULL)
	{
		if (end == NULL || (ft_strchr(WHITESPACE, *end) && in_quote == FALSE) \
		|| (ft_strchr("<>", *end) && in_quote == FALSE))
			return (end);
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
char	*get_arg(char **parsed_string, t_eggcarton *prog_info)
{
	char	*arg;
	char	*end;

	if (!*parsed_string)
		return (NULL);
	end = find_end(parsed_string);
	arg = ft_substr(*parsed_string, 0, end - *parsed_string);
	arg = check_for_expansions(prog_info, arg);
	arg = remove_quotes(arg);
	(*parsed_string) += end - *parsed_string;
	return (arg);
}

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
	new->arg_count = 0;
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
	while (index < *size)
	{
		if (array[index] != NULL)
			new[index] = array[index];
		else
			new[index] = NULL;
		index++;
	}
	return (new);
}

// static void find_args_and_redirections(char **parsed_string, t_eggcarton *prog_info, t_executable_cmd **cmd, t_cmd **head)
// {
// 	(*cmd)->args[(*cmd)->arg_count] = get_arg(parsed_string, prog_info);
// 	if (!(*cmd)->args[(*cmd)->arg_count])
// 	{
// 		printf("ERROR!!!!!!!!!!\n");//
// 	}
// 	((*cmd)->arg_count)++;
// 	if ((*cmd)->arg_count == current_size)
// 		(*cmd)->args = resize_array((*cmd)->args, &current_size);
// 	if (!**parsed_string)
// 		break ;
// 	move_pointer_past_ws(parsed_string);
// 	if (**parsed_string == '<' || **parsed_string == '>')
// 		head_cmd = handle_redirection(head_cmd, parsed_string, end, prog_info);
// }

t_cmd	*handle_exec(char **parsed_string, char *end, t_eggcarton *prog_info)
{
	t_executable_cmd	*cmd;
	t_cmd				*head_cmd;
	int					current_size;

	current_size = 10;
	cmd = new_executable_cmd();
	if (!cmd)
		return (NULL);
	head_cmd = (t_cmd *) cmd;
	move_pointer_past_ws(parsed_string);
	if (**parsed_string == '<' || **parsed_string == '>')
		head_cmd = handle_redirection(head_cmd, parsed_string, end, prog_info);
	while (*parsed_string < end && !ft_strchr("|", **parsed_string))
	{
		cmd->args[cmd->arg_count] = get_arg(parsed_string, prog_info);
		if (!cmd->args[cmd->arg_count])
		{
			printf("ERROR!!!!!!!!!!\n");//
		}
		(cmd->arg_count)++;
		if (cmd->arg_count == current_size)
			cmd->args = resize_array(cmd->args, &current_size);
		if (!**parsed_string)
			break ;
		move_pointer_past_ws(parsed_string);
		if (**parsed_string == '<' || **parsed_string == '>')
			head_cmd = handle_redirection(head_cmd, parsed_string, end, prog_info);
	}
	(prog_info->cmd_count)++;
	return (head_cmd);
}