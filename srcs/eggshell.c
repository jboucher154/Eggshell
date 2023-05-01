/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eggshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:25:23 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/28 16:31:23 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

/* Read a string, and return a pointer to it */
static char *ft_gets (void)
{
	char *line_read;

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

int eggshell(t_eggcarton *prog_info)
{
	int		status;
	char	*line;
	t_cmd	*cmd_tree;

	initialize_signals();
	status = 0;
	cmd_tree = NULL;
	while (status != 1)
	{
		line = ft_gets();
        if (line == NULL) /* Exit on Ctrl-D */
        {
			status = 1;
			break;
		//	exit(0); //EXIT CLEAN UP ETC
		} 
		if (line && *line && validate_syntax(line)) //added line check
		{
			if (!ft_strncmp("EXIT", line, 4))
			{
				status = 1;
				//clean program function needed
			}
			else
			{
				cmd_tree = parser(line, prog_info);
				print_tree(cmd_tree, 0); //
				if (cmd_tree)
					executer(cmd_tree, prog_info);
				reset_program(prog_info, &cmd_tree);
			}
		}
		if (line)
			free(line);
	}
	return (0);
	// exit(EXIT_SUCCESS);
}