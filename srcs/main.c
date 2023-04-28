/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:55:14 by smorphet          #+#    #+#             */
/*   Updated: 2023/04/28 16:25:22 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	terminal_takeover(void)
{
	struct termios takeover_term;//

	tcgetattr(STDIN_FILENO, &takeover_term);//get copy for us to edit
	takeover_term.c_lflag &= ~(ICANON);//sets to non-canonical, echoing (ICANON|ECHO)<-nonecho
	takeover_term.c_cc[VTIME] = 0;//sets timeout for non-canonical to 0
	takeover_term.c_cc[VMIN] = 0;//sets minimum input for non-canonical to 0
	tcsetattr(STDIN_FILENO, TCSANOW, &takeover_term);//set out terminal :)
	echoctl_switch(OFF); //turn off echoing of signal characters
}

int main(int argc, char **argv, char **envp)
{
	// struct termios	saved_term;
	t_eggcarton		prog_info;
	(void) argc;
	(void) argv;

	// tcgetattr(STDIN_FILENO, &saved_term);//get saved copy
	initialize_eggcarton(&prog_info, envp, NULL); //saved term
	// terminal_takeover();
	eggshell(&prog_info);

	//below this is for cleanup and exiting
	// ht_destroy(&(prog_info.environment));
	// ht_destroy(&(prog_info.command_table));
	// tcsetattr(STDIN_FILENO, TCSANOW, &saved_term);//
	return (0);
	// exit(EXIT_SUCCESS);
}
