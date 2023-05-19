/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:36:05 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 13:17:16 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * validate_pipe checks if a pipe "|" token is valid in the given context.
 * It takes a char** token and a char* str as arguments. It checks if the
 * token is surrounded by valid characters, and if not, it prints an error
 * and returns FALSE. Otherwise, it returns TRUE.
 */
static int	validate_pipe(char **token, char *str)
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
	if ((*check_before == '<' && check_before[-1] && check_before[-1] == '<'))
		return (print_blame_error(NULL, "Syntax error, unexpected token", "|"));
	if (*check_before == '<' || *check_after == '|')
		return (print_blame_error(NULL, "Syntax error, unexpected token", "|"));
	return (TRUE);
}

/*
 * validate_quotes checks if quotes are properly closed in the given context.
 * It takes a char** token as an argument and iterates over the token until
 * it finds the matching closing quote. If the quotes are not closed, it
 * prints an error and returns FALSE. Otherwise, it returns TRUE.
 */
static int	validate_quotes(char **token)
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

/*
 * validate_token validates a specific token in the given context. It takes
 * a char** token, a char* str, and a char token_id as arguments. It performs
 * validation based on the token_id, such as checking pipes, quotes, or
 * different types of redirects. If the token is invalid, it prints an error
 * and returns FALSE. Otherwise, it returns TRUE.
 */
static int	validate_token(char **token, char *str, char token_id)
{
	int		valid;

	valid = FALSE;
	if (token_id == PIPE)
		valid = validate_pipe(token, str);
	else if (**token == '"' || **token == '\'')
		valid = validate_quotes(token);
	else if (token_id == REDIRECT_OUT || token_id == REDIRECT_IN)
		valid = validate_redirect(token);
	else if (token_id == REDIRECT_OUT_APPEND || token_id == REDIRECT_HERE)
		valid = validate_redirect_out_append(token);
	return (valid);
}

/*
 * validate_syntax validates the syntax of a string in the given context.
 * It takes a char* str and a t_eggcarton* prog_info as arguments. It iterates
 * over the string, identifies tokens, and performs validation based on the
 * token type. If any validation fails, it sets the exit status in the
 * program information struct and returns FALSE. Otherwise, it returns TRUE.
 */
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
