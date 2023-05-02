/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 13:46:02 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/02 10:36:21 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//need to add in NULL checks for allocated nodes
t_cmd	*handle_pipe(char **parsed_string, char *end, t_eggcarton *prog_info)
{
	t_cmd	*cmd;

	cmd = handle_exec(parsed_string, end, prog_info);
	if (peek_next_token(*parsed_string, "|"))
	{
		move_to_token(parsed_string, end);
		(*parsed_string)++;
		move_pointer_past_ws(parsed_string);
		(prog_info->pipe_count)++;
		cmd = new_pipe(cmd, handle_pipe(parsed_string, end, prog_info));
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
