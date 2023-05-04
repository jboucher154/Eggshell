/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:14:19 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/04 09:38:35 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_redirect(char **token, char token_id)
{
	(*token)++;
	move_pointer_past_ws(token);
	printf("token at top of validate redirect: %s\n", *token);//
	if (!(**token))
		return (print_error("syntax error near unexpected token `newline'"));
	if ((token_id == REDIRECT_IN && **token == '>') || \
		(token_id == REDIRECT_OUT && **token == '<'))
		return (print_error("Syntax error, unexpected token"));
	return (TRUE);
}

int	validate_redirect_out_append(char **token)
{
	(*token) += 2;
	printf("token at top of validate redirect out append: %s\n", *token);//
	move_pointer_past_ws(token);
	if (!(**token))
		return (print_error("syntax error near unexpected token `newline'"));
	if (**token == '>' || **token == '<')
		return (print_error("Syntax error, unexpected token"));
	// if (**token == '|') //added this... <<|
	// 	return (print_error("Syntax error, unexpected token"));
	return (TRUE);
}
