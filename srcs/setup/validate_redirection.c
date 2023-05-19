/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:14:19 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 13:05:24 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * validate_redirect checks if a redirection token ("<" or ">") is valid
 * in the given context. It takes a char** token as an argument and checks
 * if the token is followed by valid characters. If not, it prints an error
 * and returns FALSE. Otherwise, it returns TRUE.
 */
int	validate_redirect(char **token)
{
	(*token)++;
	move_pointer_past_ws(token);
	if (!(**token))
		return (print_error("syntax error near unexpected token `newline'"));
	if (**token == '>' || **token == '<' || **token == '|')
		return (print_error("Syntax error, unexpected token"));
	return (TRUE);
}

/*
 * validate_redirect_out_append checks if an output redirection token
 * (">>" or ">>") is valid in the given context. It takes a char** token
 * as an argument and checks if the token is followed by valid characters.
 * If not, it prints an error and returns FALSE. Otherwise, it returns TRUE.
 */
int	validate_redirect_out_append(char **token)
{
	(*token) += 2;
	move_pointer_past_ws(token);
	if (!(**token))
		return (print_error("syntax error near unexpected token `newline'"));
	if (**token == '>' || **token == '<')
		return (print_error("Syntax error, unexpected token"));
	return (TRUE);
}
