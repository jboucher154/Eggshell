/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parse.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 14:42:17 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/02 16:06:37 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*handle_redirection(t_cmd *cmd, char **parsed_string, char *end, \
t_eggcarton *prog_info)
{
	t_cmd	*new_redir;
	int		token_id;

	new_redir = cmd;
	move_pointer_past_ws(parsed_string);
	while (*parsed_string < end && (**parsed_string == '<' || \
	**parsed_string == '>'))
	{
		token_id = move_to_token(parsed_string, end);
		if (token_id == REDIRECT_OUT_APPEND)
			(*parsed_string) += 2;
		else
			(*parsed_string)++;
		move_pointer_past_ws(parsed_string);
		if (identify_token(*parsed_string) != ALPHA)
			print_error("Error");
		new_redir = new_redirection(new_redir, parsed_string, token_id, \
		prog_info);
		move_pointer_past_ws(parsed_string);
	}
	return (new_redir);
}