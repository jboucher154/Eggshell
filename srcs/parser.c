/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 13:46:02 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/14 17:28:42 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"

//need to add in NULL checks for allocated nodes

t_cmd	*handle_pipe(char **parsed_string, char *end)
{
	t_cmd	*cmd;

	cmd = handle_exec(parsed_string, end);
	if (peek_next_token(*parsed_string, "|"))
	{
		move_to_token(parsed_string, end);
		(*parsed_string)++; //move past the token so new command can process
		cmd = new_pipe(cmd, handle_pipe(parsed_string, end));
	}
	return (cmd);
}

t_cmd	*parse_line(char **parsed_string, char *end)
{
	t_cmd	*cmd;

	printf("IN PARSE LINE\n");
	cmd = handle_pipe(parsed_string, end);
	if (peek_next_token(*parsed_string, ";")) //should I peek the next token?
	{
		move_to_token(parsed_string, end);
		(*parsed_string)++; //move past the token so new command can process
		cmd = new_line(cmd, parse_line(parsed_string, end));
	}
	return (cmd);
}


t_cmd	*parser(char *input_string)
{
	t_cmd	*cmd; //will be the head of the returned tree
	char	*parsed_string;
	char	*end_parse;

	printf("IN PARSER\n");
	parsed_string = input_string;
	end_parse = input_string + (ft_strlen(input_string));
	cmd = parse_line(&parsed_string, end_parse);
	// expand_args(&cmd); //not written
	return (cmd);
}

/*
** make initial tree √
** traverse tree and expand values if necessary, enter null terminators
** return tree head √
*/

int	main(void)
{
	t_cmd *cmd_tree = parser("< infile_me test 'this' | echo me");
	printf("head node type: %i\n", cmd_tree->type);
	if  (cmd_tree->type == 0)
	{
		printf("head node args: %s\n", ((t_executable_cmd *)cmd_tree)->args[0]);
		printf("head node args: %s\n", ((t_executable_cmd *)cmd_tree)->args[1]);
	}
	if  (cmd_tree->type == 2)
	{
		printf("infile found: %s\n", ((t_redirection*)cmd_tree)->filename);
	}
	return (0);
}