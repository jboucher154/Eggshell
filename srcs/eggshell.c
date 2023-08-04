/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eggshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:25:23 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 13:06:42 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * ft_gets() is a static function that reads a line of input from the user
 * using the readline() function and returns it as a char pointer. The function
 * also adds the input to the history and frees the memory if needed.
 */
static char	*ft_gets(void)
{
	char	*line_read;

	line_read = (char *) 0;
	if (line_read)
	{
		free(line_read);
		line_read = (char *) 0;
	}
	line_read = readline(PROMPT);
	if (line_read && *line_read)
		add_history(line_read);
	return (line_read);
}

/*
 * eggshell() is the main logic of the shell program. It takes a t_eggcarton
 * pointer as an argument. It initializes the signals, reads a line of input
 * using ft_gets(), checks for exit conditions, validates the syntax of the
 * input using validate_syntax(), and if valid, it parses and executes the
 * input using parse_and_execute(). It frees the memory allocated for the
 * input if necessary and continues the loop until an exit condition is met.
 */
static int	parse_and_execute(char **line, t_eggcarton *prog_info)
{
	t_cmd	*cmd_tree;

	cmd_tree = NULL;
	cmd_tree = parser(*line, prog_info);
	free(*line);
	*line = NULL;
	if (cmd_tree)
		executer(&cmd_tree, prog_info);
	reset_program(prog_info, &cmd_tree);
	return (0);
}

/*
 * eggshell() is the main logic of the shell program. It takes a t_eggcarton
 * pointer as an argument. It initializes the signals, reads a line of input
 * using ft_gets(), checks for exit conditions, validates the syntax of the
 * input using validate_syntax(), and if valid, it parses and executes the
 * input using parse_and_execute(). It frees the memory allocated for the
 * input if necessary and continues the loop until an exit condition is met.
 */
int	eggshell(t_eggcarton *prog_info)
{
	int		status;
	char	*line;

	status = 0;
	while (status != 1)
	{
		initialize_signals();
		line = ft_gets();
		if (line == NULL)
		{
			status = 1;
			ft_putstr_fd("Exit🐥\n", 1);
		}
		if (line && *line && validate_syntax(line, prog_info))
		{
			status = parse_and_execute(&line, prog_info);
		}
		if (line)
			free(line);
	}
	return (0);
}
