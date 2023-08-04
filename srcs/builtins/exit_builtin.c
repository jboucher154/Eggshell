/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 15:09:18 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 13:01:53 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_hash.h"

/*
*  Only digits takes and string to_ check and returns TRUE if the string is 
*  only digits and FALSE if it isn't.
*/
static int	only_digits(char *to_check)
{
	int	i;

	i = 0;
	if (to_check[0] == '-' && ft_isdigit(to_check[1]))
		i++;
	while (ft_isdigit(to_check[i]) && to_check[i] != '\0')
		i++;
	if (to_check[i] != '\0')
		return (FALSE);
	return (TRUE);
}

/*
* clean_and_restore frees all the memory allocated by the program and restores
* the terminal to its original state.
*/
void	clean_and_restore(t_eggcarton *prog_info)
{
	if (prog_info->children)
		free_children(prog_info->children);
	ht_destroy(&(prog_info->environment));
	ht_destroy(&(prog_info->command_table));
	clean_str_array(prog_info->array_env);
	if (prog_info->pipes)
		free(prog_info->pipes);
	tcsetattr(STDIN_FILENO, TCSANOW, prog_info->saved_term);
}

/*
* exit_cmd_printing prints the exit message to stdout and returns the exit code.
*/
static unsigned char	exit_cmd_printing(t_eggcarton *prog_info, t_child *cmd)
{
	unsigned char	exit_code;

	exit_code = ft_atoi(ht_get(prog_info->environment, "?"));
	if (exit_code == 0 && cmd->pid != 0)
		ft_putstr_fd("Exit🐥\n", 1);
	if (exit_code != 0 && cmd->pid != 0)
		ft_putstr_fd("Exit🍳\n", 1);
	return (exit_code);
}

/*
* exit_command takes a t_eggcarton pointer and a t_child pointer as arguments.
* It checks if the argument is a valid exit code and exits the program with the
* exit code. If the argument is not a valid exit code it prints an error and sets
* the exit code to 1.
*/
void	exit_command(t_eggcarton *prog_info, t_child *cmd)
{
	unsigned char	exit_code;

	exit_code = 0;
	if (cmd->args[1] != '\0' && cmd->args[2] == '\0')
	{
		if (!only_digits(cmd->args[1]))
		{
			print_error("Exit: numeric argument required");
			exit_code = 255;
		}
		else
			exit_code = ft_atoi(cmd->args[1]);
	}
	else if (cmd->args[2] != '\0')
	{
		print_error("Too many arguments");
		ht_update_value(prog_info->environment, "?", ft_strdup("1"));
		return ;
	}
	else
		exit_code = exit_cmd_printing(prog_info, cmd);
	clean_and_restore(prog_info);
	exit (exit_code);
}
