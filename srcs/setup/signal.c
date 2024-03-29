/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:14:03 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 15:51:05 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * signal_handler - handles signals in the shell. It sets the SIGINT signal to
 * display new line and prompt, and sets no actions for SIGQUIT
 */
static void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 1);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	return ;
}

/*
 * initialize_signals - initializes the signals for the shell using the 
 * sigaction and signal_handler function.
 */
void	initialize_signals(void)
{
	struct sigaction	sig_act;
	struct sigaction	sig_quit;

	ft_bzero(&sig_act, sizeof(sig_act));
	sig_act.sa_handler = &signal_handler;
	ft_bzero(&sig_quit, sizeof(sig_quit));
	sig_quit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sig_act, NULL);
	sigaction(SIGQUIT, &sig_quit, NULL);
}

/*
 * child_signal_handler - handles signals in the child process. It sets the 
 * SIGINT to give a newline and SIGQUIT to display "Quit: 3"
 */
static void	child_signal_handler(int sig)
{
	if (sig == SIGINT)
		write(1, "\n", 1);
	if (sig == SIGQUIT)
		write(1, " Quit: 3\n", 9);
	return ;
}

/*
 * initialize_child_signals - initializes the signals for the child process
 * using the sigaction and child_signal_handler function.
 */
void	initialize_child_signals(void)
{
	struct sigaction	sig_act;

	sig_act.sa_handler = &child_signal_handler;
	ft_bzero(&sig_act, sizeof(sig_act));
	sig_act.sa_handler = &child_signal_handler;
	sigaction(SIGINT, &sig_act, NULL);
	sigaction(SIGQUIT, &sig_act, NULL);
}
