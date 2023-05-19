/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 13:46:02 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 11:26:37 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//need to add in NULL checks for allocated nodes
t_cmd	*handle_pipe(char **parsed_string, char *end, t_eggcarton *prog_info)
{
	t_cmd	*cmd;
	t_cmd	*tmp;

	cmd = handle_exec(parsed_string, end, prog_info);
	if (!cmd)
		return (NULL);
	if (peek_next_token(*parsed_string, "|"))
	{
		move_to_token(parsed_string, end);
		(*parsed_string)++;
		move_pointer_past_ws(parsed_string);
		(prog_info->pipe_count)++;
		tmp = new_pipe(cmd, handle_pipe(parsed_string, end, prog_info));
		if (tmp == NULL || ((t_pipe *)tmp)->right == NULL)
		{
			clean_tree(cmd);
			return (NULL);
		}
		else
			cmd = tmp;
	}
	return (cmd);
}

t_cmd	*parser(char *input_string, t_eggcarton *prog_info)
{
	t_cmd	*cmd;
	char	*parsed_string;
	char	*end_parse;

	cmd = NULL;
	parsed_string = input_string;
	end_parse = input_string + (ft_strlen(input_string));
	move_pointer_past_ws(&parsed_string);
	if (parsed_string && *parsed_string)
		cmd = handle_pipe(&parsed_string, end_parse, prog_info);
	return (cmd);
}
