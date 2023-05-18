/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:36:05 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/18 10:48:32 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_pipe(char **token, char *str)
{
	char	*check_before;
	char	*check_after;

	check_before = *token;
	check_after = *token;
	(check_after)++;
	(check_before)--;
	if (*token == str)
		return (print_blame_error(NULL, "Syntax error, unexpected token", "|"));
	if (*check_after == '\0' || *check_before == '\0')
		return (print_blame_error(NULL, "Syntax error, unexpected token", "|"));
	move_pointer_backwards_ws(&check_before, str);
	move_pointer_past_ws(&check_after);
	if (*check_after == '\0' || *check_before == '\0')
		return (print_blame_error(NULL, "Syntax error, unexpected token", "|"));
	if (*check_before == ';' || (*check_before == '<' && check_before[-1] \
	&& check_before[-1] == '<'))
		return (print_blame_error(NULL, "Syntax error, unexpected token", "|"));
	if (*check_before == '<' || *check_after == '|')
		return (print_blame_error(NULL, "Syntax error, unexpected token", "|"));
	if (*check_after == ';' || *check_before == ';')
		return (print_blame_error(NULL, "Syntax error, unexpected token", "|"));
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
		valid = validate_pipe(token, str);
	else if (**token == '"' || **token == '\'')
		valid = validate_quotes(token);
	else if (**token == ';' || **token == '&')
		valid = print_error("Error, we did not do the bonus!");
	else if (token_id == REDIRECT_OUT || token_id == REDIRECT_IN)
		valid = validate_redirect(token);
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
		if (valid && *token && !((token_id == REDIRECT_IN || \
		token_id == REDIRECT_OUT || token_id == REDIRECT_OUT_APPEND || \
		token_id == REDIRECT_HERE)))
			token++;
	}
	if (valid == FALSE)
		ht_update_value(prog_info->environment, "?", ft_strdup("258"));
	return (valid);
}
