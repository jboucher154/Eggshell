/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_parse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 15:05:47 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 14:18:35 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * resize_array takes in a string array and a t_exec_parse_info struct. It
 * resizes the string array to twice its current size, and returns the new
 * string array. It also updates the current_size variable in the
 * t_exec_parse_info struct.
 */
char	**resize_array(char **array, t_exec_parse_info *parse_info)
{
	int		index;
	char	**new;

	parse_info->current_size = parse_info->current_size * 2;
	new = ft_calloc((parse_info->current_size + 1), sizeof(char *));
	if (!new)
		return (NULL);
	index = 0;
	while (array[index])
	{
		new[index] = array[index];
		index++;
	}
	while (index <= parse_info->current_size)
	{
		new[index] = NULL;
		index++;
	}
	free(array);
	return (new);
}

/*
 * find_args_and_redirections takes in a pointers to the input string,
 * the end of the input string, the t_eggcarton struct and the t_exec_parse_info 
 * struct. It finds and adds arguments and redirections to the current command.
 * It returns SUCCESS if it successfully nfinds and adds an argument or 
 * redirection, and ERROR if it fails to do so. 
 */
static int	find_args_and_redirections(char **parsed_string, char *end, \
t_eggcarton *prog_info, t_exec_parse_info *parse_info)
{
	parse_info->cmd->args[parse_info->cmd->arg_count] = \
	get_arg(parsed_string, prog_info, TRUE);
	if (parse_info->cmd->args[parse_info->cmd->arg_count])
	{
		(parse_info->cmd->arg_count)++;
	}
	if (parse_info->cmd->arg_count == parse_info->current_size)
		parse_info->cmd->args = resize_array(parse_info->cmd->args, \
		parse_info);
	if (!**parsed_string)
		return (ERROR);
	move_pointer_past_ws(parsed_string);
	if (**parsed_string == '<' || **parsed_string == '>')
		parse_info->head_cmd = handle_redirection(parse_info->head_cmd, \
		parsed_string, end, prog_info);
	return (SUCCESS);
}

/*
 * handle_exec takes in a pointer to the input string, the end of the input
 * string and the t_eggcarton struct. It creates a new command and adds it to
 * the linked list of commands. It returns a pointer to the head of the linked
 * list of commands including any redirections parsed.
 */
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
	while (parse_info.head_cmd && *parsed_string < end && !ft_strchr("|", \
	**parsed_string))
	{
		if (find_args_and_redirections(parsed_string, end, prog_info, \
		&parse_info) == ERROR)
			break ;
	}
	if (!parse_info.head_cmd)
		return (NULL);
	(prog_info->cmd_count)++;
	return (parse_info.head_cmd);
}
