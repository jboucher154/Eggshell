/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:36:05 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/20 14:53:31 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"
#define TRUE 1
#define FALSE 0


int	validate_pipe(char *token, char *str)
{
		if(token == str || token[-1] == '<' || token[-1] == ';')
		{
			printf("%sEggShell🥚: syntax error near unexpected token `|'%s\n", RED, KNRM);
			return (FALSE);
		}
		if(token[1] == ';')
		{
			printf("%sEggShell🥚: syntax error near unexpected token `|'%s\n", RED, KNRM);
			return (FALSE);
		}
		if(token[-1] == '<' && token[-2] == '<')
		{
			printf("%sEggShell🥚: syntax error near unexpected token `|'%s\n", RED, KNRM);
			return (FALSE);
		}
 	return (TRUE);
}

// void  validate_quotes(char *line, char symbol)
// {
    //check for matching parenthases, quotes.
// }

// int	validate_redirect_in(char *token_start)
// {
// 	return (TRUE);
// }

// int	validate_redirect_out(char *token_start)
// {
// 	return (TRUE);
// }

// int	validate_redirect_out_append(char *token_start)
// {
// 	return (TRUE);
// }

int	validate_syntax(char *str)
{
		char token_id;
		char *token;
		int valid;

		token = str;
		valid = TRUE;
		while (token && valid)
		{
			printf("\ninside loop\n");
			token = ft_strchr(TOKENS, *token);
			if (!token)
				break ;
			token_id = identify_token(token);
			if (token_id == PIPE)
			{
				printf("\n entering validate pipe\n");
				valid = validate_pipe(token, str);	//validate pipe syntax
			}
			// else if (token_id == REDIRECT_IN)
			// 	valid = validate_redirect_in(token_start);	//validate redirect in syntax
			// else if (token_id == REDIRECT_OUT)
			// 	valid = validate_redirect_out(token_start);	//validate redirect out syntax
			// else if (token_id == REDIRECT_OUT_APPEND)
			// 	valid = validate_redirect_out_append(token_start);	//validate redirect out append syntax
			// else if (token_id == REDIRECT_HERE)
			// 	valid = validate_redirect_out_append(token_start);	//validate redirect out append syntax
			// if (token == REDIRECT_OUT_APPEND || token == REDIRECT_HERE)
			// 	token = token + 2;
			// else 
				token++;
		}
		// if (token_id == ALPHA)
		// 	valid = validate_commands(token_start); //validate commands syntax
		return (valid);
}

int main(void)
{
	char *str = "how does this print";

	printf("%i\n", validate_syntax(str));
	return (0);
}