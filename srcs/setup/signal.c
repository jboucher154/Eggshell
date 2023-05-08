/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:14:03 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/02 16:03:45 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_signals(void)
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

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 1);
		write(1, "\n", 1);
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

static void	child_signal_handler(int sig)
{
	if (sig == SIGINT)
		write(1, "\n", 1);
	if (sig == SIGQUIT)
	{
		write(1, " Quit: 3\n", 9);
	}
	return ;
}

void	initialize_child_signals(void)
{
	struct sigaction	sig_act;
	
	sig_act.sa_handler = &child_signal_handler;
	ft_bzero(&sig_act, sizeof(sig_act));
	sig_act.sa_handler = &child_signal_handler;
	sigaction(SIGINT, &sig_act, NULL);
	sigaction(SIGQUIT, &sig_act, NULL);
}

static void	heredoc_signal_handler(int sig)
{
    if (sig == SIGINT)
		exit (0);
}

void	initialize_heredoc_signals(void)
{
	struct sigaction	sig_act;
	struct sigaction	sig_quit;

	ft_bzero(&sig_act, sizeof(sig_act));
	sig_act.sa_handler = &heredoc_signal_handler;
	ft_bzero(&sig_quit, sizeof(sig_quit));
	sig_quit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sig_act, NULL);
	sigaction(SIGTSTP, &sig_act, NULL);
	sigaction(SIGQUIT, &sig_quit, NULL);
}


