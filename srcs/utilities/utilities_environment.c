/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_environment.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 14:37:52 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 14:05:51 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * update_environment_array updates the environment array in the t_eggcarton
 * struct based on the current state of the environment hash table. It takes
 * a t_eggcarton* prog_info as an argument. It exports the environment hash
 * table to a new string array using ht_export_to_array(), frees the memory
 * allocated for the old array using clean_str_array(), and assigns the new
 * array to the array_env pointer in the t_eggcarton struct.
 */
void	update_environment_array(t_eggcarton *prog_info)
{
	char	**new_enviornmnet;

	new_enviornmnet = ht_export_to_array(prog_info->environment);
	if (!new_enviornmnet)
	{
		print_error("malloc error: environment not updated");
		return ;
	}
	clean_str_array(prog_info->array_env);
	prog_info->array_env = new_enviornmnet;
}

/*
 * echoctl_switch toggles the ECHOCTL flag in the terminal settings. It takes
 * an int toggle as an argument. If toggle is OFF, it clears the ECHOCTL flag,
 * disabling the printing of control characters. If toggle is any other value,
 * it sets the ECHOCTL flag, enabling the printing of control characters. It
 * modifies the terminal settings using tcsetattr().
 */
void	echoctl_switch(int toggle)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	if (toggle == OFF)
	{
		term.c_lflag &= ~(ECHOCTL);
	}
	else
	{
		term.c_lflag |= ECHOCTL;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
