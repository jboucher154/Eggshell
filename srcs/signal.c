/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:14:03 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/28 16:11:54 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void signal_handler(int sig) //t_eggcarton *prog_info
{
	
	if (sig == SIGINT)
	{
		rl_replace_line("", 1);
	 	write(1,"\n", 1);
		rl_on_new_line();
		return ;
	}
	if (sig == SIGTSTP)
	{
		printf("%i\n", sig);
		printf("No Sheree, not that one!!\n");
	}
	if (sig == SIGQUIT)
	{
		// (void) prog_info;
		//clean exit program
		// ht_destroy(&(prog_info->environment));
		// ht_destroy(&(prog_info->command_table));
		// tcsetattr(STDIN_FILENO, TCSANOW, prog_info->saved_term);//
		exit(EXIT_SUCCESS);
	}
	return ;
}

