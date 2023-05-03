/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eggshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:25:23 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/03 15:24:07 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Read a string, and return a pointer to it */
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

//add clean program exit when exited
static int	parse_and_execute(char **line, t_eggcarton *prog_info)
{
	t_cmd	*cmd_tree;
	int		status;

	status = 0;
	cmd_tree = NULL;
	if (!ft_strncmp("EXIT", *line, 4))
	{
		status = 1;
		//clean program function needed
	}
	else
	{
		cmd_tree = parser(*line, prog_info);
		print_tree(cmd_tree, 0); //
		free(*line);
		*line  = NULL;
		if (cmd_tree)
			executer(&cmd_tree, prog_info);
		reset_program(prog_info, &cmd_tree);
	}
	return (status);
}

//need to add clean exit for ctl-D, etc
int	eggshell(t_eggcarton *prog_info)
{
	int		status;
	char	*line;

	initialize_signals();
	status = 0;
	while (status != 1)
	{
		line = ft_gets();
        if (line == NULL) /* Exit on Ctrl-D */
		{
			status = 1;
			break ;
		//	exit(0); //EXIT CLEAN UP ETC
		}
		if (line && *line && validate_syntax(line, prog_info))
		{
			status = parse_and_execute(&line, prog_info);
		}
		if (line)
			free(line);
	}
	exit(EXIT_SUCCESS);
}
