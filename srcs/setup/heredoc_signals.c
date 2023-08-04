/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:14:03 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 15:36:51 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * heredoc_signal_handler - handles signals in heredoc (<<)
 * functions. It sets the SIGINT signal to exit the program and 
 * sets not actions for SIGTSTP and SIGQUIT
*/
static void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
		exit (5);
}

/*
 * initialize_heredoc_signals - initializes the signals for the heredoc (<<)
 * functions. It sets the SIGINT signal to exit the program
 */
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
