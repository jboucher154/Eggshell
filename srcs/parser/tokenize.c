/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 10:51:35 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 14:34:37 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * id_token takes in a pointer to the token start and returns the token id
 * associated with the token.
 */
char	identify_token(char *token_start)
{
	char	token_id;

	token_id = '\0';
	if (*token_start == '\0')
		return (token_id);
	else if (!ft_strchr(TOKENS, *token_start))
		token_id = ALPHA;
	else if (*token_start == '|')
		token_id = PIPE;
	else if (*token_start == '<')
	{
		if (token_start + 1 && *(token_start + 1) == '<')
			token_id = REDIRECT_HERE;
		else
			token_id = REDIRECT_IN;
	}
	else if (*token_start == '>')
	{
		if (token_start + 1 && *(token_start + 1) == '>')
			token_id = REDIRECT_OUT_APPEND;
		else
			token_id = REDIRECT_OUT;
	}
	return (token_id);
}

/*
 * move_to_token takes in a pointer to the parsed string and a pointer to the
 * end of the string. It moves the parsed string past any whitespace and returns
 * the token id of the next token.
 */
char	move_to_token(char **parsed_string, char *end)
{
	char	token_id;

	while ((*parsed_string) < end)
	{
		if (ft_strchr(WHITESPACE, **parsed_string))
			(*parsed_string)++;
		else
			break ;
	}
	token_id = identify_token(*parsed_string);
	return (token_id);
}

/*
** Find the next token in the input string
** If token not found, return FALSE
** If token found, return TRUE
*/
int	peek_next_token(char *current_index, char *look_for)
{
	char	*token;

	token = current_index;
	while (token)
	{
		if (ft_strchr(TOKENS, *token))
			break ;
		token++;
	}
	return (*token != '\0' && ft_strchr(look_for, *token));
}
