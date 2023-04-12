/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:36:05 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/12 14:50:11 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"

int	validate_alpha(char *token_start)
{
	return (TRUE);
}

int	validate_pipe(char *token_start)
{
	return (TRUE);
}

int	validate_or(char *token_start)
{
	return (TRUE);
}

int	validate_and(char *token_start)
{
	return (TRUE);
}

int	validate_redirect_in(char *token_start)
{
	return (TRUE);
}

int	validate_redirect_out(char *token_start)
{
	return (TRUE);
}

int	validate_redirect_out_append(char *token_start)
{
	return (TRUE);
}

int	validate_semicolon(char *token_start)
{
	return (TRUE);
}

int	validate_open_parenthesis(char *token_start)
{
	return (TRUE);
}

int	validate_close_parenthesis(char *token_start)
{
	return (TRUE);
}

int	validate_syntax(char *token_start, char token_id)
{
	int	valid;

	if (token_id == ALPHA)
		valid = validate_alpha(token_start); //validate alpha syntax
	else if (token_id = PIPE)
		valid = validate_pipe(token_start);	//validate pipe syntax
	else if (token_id = OR)
		valid = validate_or(token_start);	//validate or syntax
	else if (token_id = AND)
		valid = validate_and(token_start);	//validate and syntax
	else if (token_id = REDIRECT_IN)
		valid = validate_redirect_in(token_start);	//validate redirect in syntax
	else if (token_id = REDIRECT_OUT)
		valid = validate_redirect_out(token_start);	//validate redirect out syntax
	else if (token_id = REDIRECT_OUT_APPEND)
		valid = validate_redirect_out_append(token_start);	//validate redirect out append syntax
	else if (token_id = SEMICOLON)
		valid = validate_semicolon(token_start);	//validate semicolon syntax
	else if (token_id = OPEN_PARENTHESIS)
		valid = validate_open_parenthesis(token_start);	//validate open parenthesis syntax
	else if (token_id = CLOSE_PARENTHESIS)
		valid = validate_close_parenthesis(token_start);	//validate close parenthesis syntax
	return (valid);
}