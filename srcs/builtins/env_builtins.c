/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 14:14:27 by smorphet          #+#    #+#             */
/*   Updated: 2023/05/10 09:48:18 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_enviroment(t_hash_table	*ht_env)
{
	ht_print_env(ht_env);
}

void	unset_command(char **args, t_eggcarton *prog_info)
{
	int	index;

	index = 1;
	while (args[index] != '\0')
	{	
		if (is_valid_var_name(args[index]) == FALSE)
		{
			print_blame_error("unset", "not a valid identifier", args[index]);
			ht_update_value(prog_info->environment, "?", ft_itoa(1));
		}
		if (!strncmp(args[index], "PWD", 3))
			{
				index++;
				continue;
			}
		else
			ht_remove(prog_info->environment, args[index]);
		index++;
	}
	update_environment_array(prog_info);
}
