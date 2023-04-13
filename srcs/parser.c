/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 13:46:02 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/13 09:55:08 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"

/*
**	Identify the token type and return the token id assigned in the enum
*/
char	identify_token(char *token_start)
{
	char	token_id;

	if (token_start == NULL)
		return (token_start);
	if (!ft_strchr(TOKENS, *token_start) && !ft_strchr(WHITESPACE, *token_start))
		return (ALPHA);
	else if (token_start == '|')
	{
		if (token_start + 1 && *(token_start + 1 ) == '|')
			token_id = OR;
		else
			token_id = PIPE;
	}
	else if (*token_start == '&')
	{
		if (token_start + 1 && *(token_start + 1 ) == '&')
			token_id = AND;
		else
			token_id = ALPHA; //???
	}
	else if (*token_start == '<')
		token_id = REDIRECT_IN;
	else if (*token_start == '>')
	{
		if (token_start + 1 && *(token_start + 1 ) == '>')
			token_id = REDIRECT_OUT_APPEND;
		else
			token_id = REDIRECT_OUT;
	}
	else if (*token_start == ';')
		token_id = SEMICOLON;
	else if (*token_start == '(')
		token_id = OPEN_PARENTHESIS;
	else if (*token_start == ')')
		token_id = CLOSE_PARENTHESIS;

}

char	*find_next_token(char *input_string, int *index)
{
	char	*token;
	
	
	token  = input_string + (*index);
	while(token)
	{
		if (ft_strchr(TOKENS, *token)) //if token is found, break and advance index
		{
			(*index) += 1;
			return (token);
		}
		else if (ft_strchr(WHITESPACE, *token)) //if whitespace is found, advance index past it, if multiple whitespace, subsequent passes of while loop should get past them all
		{
			(*index) += 1;
			token++;
		}
		else
			break;
		// token++; //don't think this needs to increment
	}
	return (token);
}


/*
**	This is not set up correctly. I'm wondering if there should be a function it calls that can be recursive and continue to setup nodes?
** not sure of the design yet. For example a pipe node should also have a left and right child, they should be set up on the correct pipe
** node. I guess that why its called a recursive descent parser :). For now I am making support structure for the main recursive engine
*/
t_cmd	*parser(char *input_string)
{
	t_cmd	*cmd; //will be the head of the returned tree
	int		index; //track where we are in the input string, account for find file names, args, etc
	char	*token_start;
	char	token_id;

	index = 0;
	cmd = NULL;
	while(input_string[index])
	{
		token_start = find_next_token(input_string, &index);
		if (token_start == NULL)
			break;
		token_id = identify_token(token_start);
		if (validate_syntax(token_start, token_id) == FALSE)
		{
			ft_putstr_fd("Syntax error", 1); //we can handle the error message better later
			return (NULL); // should free tree before returning?
		}
		//make new node and add to tree
		//bail if the node creation fails, if head is ever NULL after first addition
	}
	

	return (cmd);
}

/*
** make initial tree
** traverse tree and expand values if necessary, enter null terminators
** return tree head
*/