/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:36:05 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/03 15:37:42 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_pipe(char *token, char *str)
{
	if (token == str || token[-1] == '<' || token[-1] == ';' || \
		(token[1] && token[1] == ';') || (!token[1]))
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
	if (!(**token))
		return (print_error("Syntax error, unclosed quotes"));
	else
		return (TRUE);
}

static int	validate_token(char **token, char *str, char token_id)
{
	int		valid;

	valid = FALSE;
	if (token_id == PIPE)
		valid = validate_pipe(*token, str);
	else if (**token == '"' || **token == '\'')
		valid = validate_quotes(token);
	else if (**token == ';' || **token == '&')
		valid = print_error("Error, we did not do the bonus!");
	else if (token_id == REDIRECT_OUT || token_id == REDIRECT_IN)
		valid = validate_redirect(token, token_id);
	else if (token_id == REDIRECT_OUT_APPEND || token_id == REDIRECT_HERE)
		valid = validate_redirect_out_append(token);
	return (valid);
}

int	validate_syntax(char *str, t_eggcarton *prog_info)
{
	char	token_id;
	char	*token;
	int		valid;

	token = str;
	valid = TRUE;
	while (*token && valid)
	{
		while (*token && ft_strchr(TOKENS, *token) == NULL && \
		ft_strchr(QUOTES, *token) == NULL)
			token++;
		if (!*token)
			break ;
		token_id = identify_token(token);
		valid = validate_token(&token, str, token_id);
		if (valid && !((token_id == REDIRECT_IN || token_id == REDIRECT_OUT || \
		token_id == REDIRECT_OUT_APPEND) && ft_strchr(QUOTES, *token)))
			token++;
	}
	if (valid == FALSE)
		ht_update_value(prog_info->environment, "?", ft_strdup("258"));
	return (valid);
}
