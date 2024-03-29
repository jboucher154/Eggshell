/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:55:14 by smorphet          #+#    #+#             */
/*   Updated: 2023/05/19 13:02:29 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Takes over the terminal by modifying terminal attributes and 
*  disabling echoing of control characters.
*/
void	terminal_takeover(void)
{
	struct termios	takeover_term;

	tcgetattr(STDIN_FILENO, &takeover_term);
	tcsetattr(STDIN_FILENO, TCSANOW, &takeover_term);
	echoctl_switch(OFF);
}

int	main(int argc, char **argv, char **envp)
{
	struct termios	saved_term;
	t_eggcarton		prog_info;

	(void) argc;
	(void) argv;
	tcgetattr(STDIN_FILENO, &saved_term);
	initialize_eggcarton(&prog_info, envp, &saved_term);
	terminal_takeover();
	eggshell(&prog_info);
	clean_and_restore(&prog_info);
	return (0);
}
