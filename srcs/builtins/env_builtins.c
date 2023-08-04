/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 14:14:27 by smorphet          #+#    #+#             */
/*   Updated: 2023/05/19 12:59:18 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* print_environment prints the environment to stdout using the hashtable 
* printing function. It returns SUCCESS always.
*/
int	print_enviroment(t_hash_table	*ht_env)
{
	ht_print_env(ht_env);
	return (SUCCESS);
}

/*
* unset_command takes a char** of arguments and removes them to the environment
* hashtable. If the argument is not a valid variable name it prints an error and 
* sets the exit status to ERROR. It returns the exit status.
*/
int	unset_command(char **args, t_eggcarton *prog_info)
{
	int	index;
	int	exit_status;

	index = 1;
	exit_status = SUCCESS;
	while (args[index] != '\0')
	{	
		if (is_valid_var_name(args[index]) == FALSE)
		{
			print_blame_error("unset", "not a valid identifier", args[index]);
			exit_status = ERROR;
		}
		if (!strncmp(args[index], "PWD", 3))
		{
			index++;
			continue ;
		}
		else
			ht_remove(prog_info->environment, args[index]);
		index++;
	}
	update_environment_array(prog_info);
	return (exit_status);
}
