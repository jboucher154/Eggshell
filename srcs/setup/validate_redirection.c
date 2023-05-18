/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:14:19 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/18 10:48:13 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_redirect(char **token)
{
	(*token)++;
	move_pointer_past_ws(token);
	if (!(**token))
		return (print_error("syntax error near unexpected token `newline'"));
	if (**token == '>' || **token == '<')
		return (print_error("Syntax error, unexpected token"));
	return (TRUE);
}

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
