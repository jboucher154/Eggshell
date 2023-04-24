/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:36:05 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/24 16:31:41 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"
#define TRUE 1
#define FALSE 0

int	validate_pipe(char *token, char *str)
{
		if(token == str || token[-1] == '<' || token[-1] == ';' || \
			token[1] == ';')
		{
			ft_putstr_fd("\033[31mEggShell🥚: syntax error near unexpected token `|'\x1B[0m\n", 2);
			return (FALSE);
		}
		if (token[1] == '|')
		{
			ft_putstr_fd("\033[31mEggShell🥚: Error, we did not do the bonus!\x1B[0m\n", 2);
			return (FALSE);	
		}
		if(token[-1] == '<' && token[-2] == '<')
		{
			ft_putstr_fd("\033[31mEggShell🥚: syntax error near unexpected token `|'\x1B[0m\n", 2);
			return (FALSE);
		}
 	return (TRUE);
}

int	validate_quotes(char **token)
{
	char	quote_to_match;

	quote_to_match = **token;
	(*token)++;
	while (**token && **token != quote_to_match)
		(*token)++;
	if(!**token)
	{
		ft_putstr_fd("\033[31mEggShell🥚: syntax error unclosed quotes\x1B[0m\n", 2);
		return (FALSE);
	}
	else
		return (TRUE);
}

int	validate_redirect_in(char *token)
{
	printf("redirect in entered\n");
	if (ft_strchr(WHITESPACE, token[1]) == NULL && ft_isalpha(token[1]) == 0 && token[1] != '$')
	{
		ft_putstr_fd("\033[31mEggShell🥚: syntax error near unexpected token `>'\x1B[0m\n", 2);
		return (FALSE);
	}
	return (TRUE);
}

int	validate_redirect_out(char *token)
{
		printf("redirect out entered\n");
	if (ft_strchr(WHITESPACE, token[1]) == NULL && ft_isalpha(token[1]) != 1 && \
		token[1] != '$')
	{
		ft_putstr_fd("\033[31mEggShell🥚: syntax error near unexpected token `>'\x1B[0m\n", 2);
		return (FALSE);
	}
	return (TRUE);
}

int	validate_redirect_out_append(char *token)
{
	if (ft_strchr(WHITESPACE, token[0]) == NULL && ft_isalpha(token[0]) == 0 && \
		token[0] != '$')
	{
		ft_putstr_fd("\033[31mEggShell🥚: syntax error near unexpected token `>'\x1B[0m\n", 2);
		return (FALSE);
	}
	return (TRUE);
}

int	validate_syntax(char *str)
{
		char token_id;
		char *token;
		int valid;
		
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
			else if (token_id == REDIRECT_IN)
				valid = validate_redirect_in(token); //validate redirect in syntax
			else if (token_id == REDIRECT_OUT)
				valid = validate_redirect_out(token);	//validate redirect out syntax
			if (token_id == REDIRECT_OUT_APPEND || token_id == REDIRECT_HERE)
			{
				token = token + 2;
				valid = validate_redirect_out_append(token);
			}
			if (valid)
				token++;
		}
		return (valid);
}