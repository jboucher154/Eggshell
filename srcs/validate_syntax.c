/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:36:05 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/25 10:53:21 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"

int	print_error(char *error_msg)
{
	ft_putstr_fd("\033[31mEggShell🥚: ", 2);
	ft_putstr_fd(error_msg, 2);
	ft_putstr_fd("\x1B[0m\n", 2);
	return (FALSE);
}

int	validate_pipe(char *token, char *str)
{
	if (token == str || token[-1] == '<' || token[-1] == ';' || \
		token[1] == ';')
		return (print_error("Syntax error, unexpected token"));
	if (token[1] == '|')
		return (print_error("Error, we did not do the bonus!"));
	if (token[-1] == '<' && token[-2] == '<')
		return (print_error("Syntax error, unexpected token"));
	return (TRUE);
}

int	validate_quotes(char **token)
{
	char	quote_to_match;

	quote_to_match = **token;
	(*token)++;
	while (**token && **token != quote_to_match)
		(*token)++;
	if (!**token)
		return (print_error("Syntax error, unclosed quotes"));
	else
		return (TRUE);
}

int	validate_redirect(char **token, char token_id)
{
	(*token)++;
	if (!*token)
		return (print_error("syntax error near unexpected token `newline'"));
	move_pointer_past_ws(token);
	// if (ft_isascii(token[1]) == 0)
	// 	return (print_error("Syntax error, unexpected token"));
	if ((token_id == REDIRECT_IN && **token == '>') || (token_id == REDIRECT_OUT && **token == '<'))
		return (print_error("Syntax error, unexpected token"));
	return (TRUE);
}

int	validate_redirect_out_append(char **token)
{
	(*token) += 2;
	move_pointer_past_ws(token);
	if (!*token)
		return (print_error("syntax error near unexpected token `newline'"));
	// if (ft_isascii(token) == 0)
	// 	return (print_error("Syntax error, unexpected token"));
	if (**token == '>' || **token == '<')
		return (print_error("Syntax error, unexpected token"));
	return (TRUE);
}

int	validate_syntax(char *str)
{
	char	token_id;
	char	*token;
	int		valid;

	token = str;
	valid = 4;
	while (*token && valid)
	{
		while (*token && ft_strchr(TOKENS, *token) == NULL && ft_strchr(QUOTES, *token) == NULL)
			token++;
		if (!*token)
			break ;
		token_id = identify_token(token);
		if (token_id == PIPE)
			valid = validate_pipe(token, str);
		else if (*token == '"' || *token == '\'')
			valid = validate_quotes(&token);
		else if (*token == ';' || *token == '&')
			valid = print_error("Error, we did not do the bonus!");
		else if (token_id == REDIRECT_OUT || token_id == REDIRECT_IN)
			valid = validate_redirect(&token, token_id);
		if (token_id == REDIRECT_OUT_APPEND || token_id == REDIRECT_HERE)
			valid = validate_redirect_out_append(&token);
		if (valid)
			token++;
	}
	return (valid);
}
