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


void initialize_signals(void)
{
	struct sigaction	sig_act;
	struct sigaction	sig_quit;

	ft_bzero(&sig_act, sizeof(sig_act));
    sig_act.sa_handler = &signal_handler;
	ft_bzero(&sig_quit, sizeof(sig_quit));
    sig_quit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sig_act, NULL);
	sigaction(SIGTSTP, &sig_act, NULL);
   	sigaction(SIGQUIT, &sig_quit, NULL);
}


void signal_handler(int sig) //t_eggcarton *prog_info
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 1);
	 	write(1,"\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGTSTP)
	{
		printf("%i\n", sig);
		printf("No Sheree, not that one!!\n");
	}
	return ;
}

