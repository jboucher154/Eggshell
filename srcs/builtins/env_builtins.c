/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 14:14:27 by smorphet          #+#    #+#             */
/*   Updated: 2023/05/03 10:43:45 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_enviroment(t_hash_table	*ht_env)
{
	ht_print_env(ht_env);
}

void	unset_command(char **args, t_hash_table *ht_env)
{
	int	index;

	index = 1;
	while (args[index] != '\0')
	{	
		ht_remove(ht_env, args[index]);
		index++;
	}
}

void	set_new_env_variable(char *arg, t_hash_table *environment)
{
	int		arg_index;
	char	*key;
	char	*value;

	arg_index = 0;
	while (arg[arg_index] != '\0' && arg[arg_index] != '=')
		arg_index++;
	key = ft_substr(arg, 0, arg_index);
	if (arg[arg_index] == '\0')
		value = NULL;
	else
	{
		arg_index++;
		value = ft_strdup(arg + arg_index);
	}
	ht_add(environment, key, value);
	free(key);
}

void	export_command(char **args, t_hash_table *environment)
{
	int	index;
	int	added;

	added = 0;
	index = 1;
	while (args[index])
	{
		if (args[index][0])
		{
			set_new_env_variable(args[index], environment);
			added++;
		}
		index++;
	}
	if (added == 0)
		ht_print_export(environment);
}
