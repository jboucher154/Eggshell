/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 10:51:35 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/17 12:59:20 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"

/*
**	Identify the token type and return the token id assigned in the enum
*/
char	identify_token(char *token_start)
{
	char	token_id;

	if (*token_start == '\0')
		return (*token_start);
	else if (!ft_strchr(TOKENS, *token_start))
		token_id = ALPHA;
	else if (*token_start == '|')
		token_id = PIPE;
	else if (*token_start == '<')
		token_id = REDIRECT_IN;
	else if (*token_start == '>')
	{
		if (token_start + 1 && *(token_start + 1 ) == '>')
			token_id = REDIRECT_OUT_APPEND;
		else
			token_id = REDIRECT_OUT;
	}
	else if (*token_start == ';')
		token_id = SEMICOLON;
	return (token_id);
}

//return the next non-whitespace character's identity
char	move_to_token(char **parsed_string, char *end)
{
	char token_id;

	while((*parsed_string) < end)
	{
		if (ft_strchr(WHITESPACE, **parsed_string)) //if whitespace is found, advance index past it, if multiple whitespace, subsequent passes of while loop should get past them all
			(*parsed_string)++;
		else
			break;
	}
	token_id = identify_token(*parsed_string);
	// printf("token found: %c, first char of string: %c\n", token_id, **parsed_string);//
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
	while(token)
	{
		if (ft_strchr(TOKENS, *token)) //if token is found, break and advance index
			break ;
		token++;
	}
	// printf("result of peek: %i\n", (*token != '\0' && ft_strchr(look_for, *token)));//
	return (*token != '\0' && ft_strchr(look_for, *token)); //if not null and in the token string should return true
}
