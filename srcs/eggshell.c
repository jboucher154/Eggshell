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

	struct sigaction sig_act;//
	ft_bzero(&sig_act, sizeof(sig_act));

	sig_act.sa_handler = &signal_handler;
	sig_act.sa_flags = SA_RESTART;
	
	// (void) prog_info;
	sigaction(SIGINT, &sig_act, NULL);
	sigaction(SIGTSTP, &sig_act, NULL);
	sigaction(SIGQUIT, &sig_act, NULL);

	status = 0;
	// status = -5;
	cmd_tree = NULL;
	while (status != 1)
	{
		printf("BEFORE FT GETES\n");
		line = ft_gets();
		printf("LINE: %s\n", line);
		sleep(4);//
		// status += 1;
		if (line && *line && validate_syntax(line)) //added line check
		{	
			printf("AFTER FT GETES\n");
			if (!ft_strncmp("EXIT", line, 4))
			{
				status = 1;
				//clean program function needed
			}
			else
			{
		// 		//validate syntax
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