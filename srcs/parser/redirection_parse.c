/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parse.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 14:42:17 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 14:33:31 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * postion_string takes in an int representing the token id and a pointer to the
 * parsed string. It moves the parsed string past the token and any whitespace.
 */
static void	postion_string(int token_id, char **parsed_string)
{
	if (token_id == REDIRECT_OUT_APPEND || token_id == REDIRECT_HERE)
		(*parsed_string) += 2;
	else
		(*parsed_string)++;
	move_pointer_past_ws(parsed_string);
}

/*
 * handle_redirection takes in pointers to the command tree, the parsed string,
 * the end of the string, and the t_eggcarton struct. It creates a new
 * redirection struct and adds it to the command tree. If malloc fails, it
 * frees the tree and returns NULL.
 */
t_cmd	*handle_redirection(t_cmd *cmd, char **parsed_string, char *end, \
t_eggcarton *prog_info)
{
	t_cmd	*new_redir;
	t_cmd	*tmp;
	int		token_id;

	new_redir = cmd;
	move_pointer_past_ws(parsed_string);
	while (*parsed_string < end && (**parsed_string == '<' || \
	**parsed_string == '>'))
	{
		token_id = move_to_token(parsed_string, end);
		postion_string(token_id, parsed_string);
		tmp = new_redirection(new_redir, parsed_string, token_id, \
		prog_info);
		if (!tmp)
			break ;
		else
			new_redir = tmp;
		move_pointer_past_ws(parsed_string);
	}
	if (tmp == NULL)
	{
		clean_tree(new_redir);
		return (NULL);
	}
	return (new_redir);
}
