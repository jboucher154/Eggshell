/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_parse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 15:05:47 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/03 09:54:27 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	find_args_and_redirections(char **parsed_string, char *end, \
t_eggcarton *prog_info, t_exec_parse_info *parse_info)
{
	parse_info->cmd->args[parse_info->cmd->arg_count] = \
	get_arg(parsed_string, prog_info);
	if (!parse_info->cmd->args[parse_info->cmd->arg_count])
	{
		return (print_error("Malloc failed in agument parsing."));
	}
	(parse_info->cmd->arg_count)++;
	if (parse_info->cmd->arg_count == parse_info->current_size)
		parse_info->cmd->args = resize_array(parse_info->cmd->args, \
		&(parse_info->current_size));
	if (!**parsed_string)
		return (ERROR);
	move_pointer_past_ws(parsed_string);
	if (**parsed_string == '<' || **parsed_string == '>')
		parse_info->head_cmd = handle_redirection(parse_info->head_cmd, \
		parsed_string, end, prog_info);
	return (SUCCESS);
}

t_cmd	*handle_exec(char **parsed_string, char *end, t_eggcarton *prog_info)
{
	t_exec_parse_info	parse_info;

	parse_info.current_size = 10;
	parse_info.cmd = new_executable_cmd();
	if (!parse_info.cmd)
		return (NULL);
	parse_info.head_cmd = (t_cmd *) parse_info.cmd;
	move_pointer_past_ws(parsed_string);
	if (**parsed_string == '<' || **parsed_string == '>')
		parse_info.head_cmd = handle_redirection(parse_info.head_cmd, \
		parsed_string, end, prog_info);
	while (*parsed_string < end && !ft_strchr("|", **parsed_string))
	{
		if (find_args_and_redirections(parsed_string, end, prog_info, \
		&parse_info) == ERROR)
			break ;
	}
	(prog_info->cmd_count)++;
	return (parse_info.head_cmd);
}
