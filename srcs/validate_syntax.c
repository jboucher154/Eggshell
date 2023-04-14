/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:36:05 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/14 17:13:17 by jebouche         ###   ########.fr       */
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

//maybe this driver isn't needed? can call a validation everytime we ID a token in the parser?
int	validate_syntax(char *token_start, char token_id)
{
	int	valid;

	if (token_id == ALPHA)
		valid = validate_alpha(token_start); //validate alpha syntax
	else if (token_id == PIPE)
		valid = validate_pipe(token_start);	//validate pipe syntax
	else if (token_id == REDIRECT_IN)
		valid = validate_redirect_in(token_start);	//validate redirect in syntax
	else if (token_id == REDIRECT_OUT)
		valid = validate_redirect_out(token_start);	//validate redirect out syntax
	else if (token_id == REDIRECT_OUT_APPEND)
		valid = validate_redirect_out_append(token_start);	//validate redirect out append syntax
	else if (token_id == SEMICOLON)
		valid = validate_semicolon(token_start);	//validate semicolon syntax
	return (valid);
}