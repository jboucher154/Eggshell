/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:16:07 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 15:39:29 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * set_shell_level - sets the shell level in the environment hash table
 * if it doesn't exist, it creates it and sets it to 1. If it exists, it
 * increments it by 1. If it is greater than 1000, it sets it to 1.
 */
int	set_shell_level(t_hash_table *environment)
{
	char	*level;
	char	*new_level;
	int		int_level;

	level = ht_get(environment, "SHLVL");
	if (!level)
	{
		level = ft_itoa(1);
		if (!level)
			return (ERROR);
		if (ht_add(environment, "SHLVL", level) == ERROR)
			return (ERROR);
		return (SUCCESS);
	}
	int_level = ft_atoi(level) + 1;
	if (int_level > 1000)
		int_level = 1;
	new_level = ft_itoa(int_level);
	if (!new_level)
		return (ERROR);
	return (ht_update_value(environment, "SHLVL", new_level));
}

/*
 * initialize_env_table - initializes the environment hash table with the
 * commmand line envp** passed to the program
 */
static void	initialize_env_table(t_hash_table *ht_env, char **envp)
{
	int		index;
	int		out_index;
	char	*key;
	char	*value;

	index = 0;
	out_index = 0;
	while (envp[out_index])
	{
		while (envp[out_index][index] != '=')
			index++;
		key = ft_substr(envp[out_index], 0, index);
		index++;
		value = ft_strdup(envp[out_index] + index);
		ht_add(ht_env, key, value);
		free(key);
		index = 0;
		out_index++;
	}
	ht_add(ht_env, "?", ft_strdup("0"));
	ht_update_value(ht_env, "SHELL", getcwd(NULL, 0));
	if (set_shell_level(ht_env) == ERROR)
		print_error("Shell level not set!");
}

/*
 * initialize_eggcarton - initializes the eggcarton structure with the
 * environment hash table, the command table and the array of environment
 * variables. If any of these initializations fail, it prints message
 * and returns ERROR.
 */
int	initialize_eggcarton(t_eggcarton *prog, char **env, struct termios *term)
{
	prog->environment = ht_create(150);
	if (prog->environment == NULL)
	{	
		print_error("Environment initialization failed\n");
		return (ERROR);
	}
	initialize_env_table(prog->environment, env);
	if (initalize_command_table(prog) == ERROR)
	{	
		print_error("Command table initialization failed\n");
		return (ERROR);
	}
	prog->array_env = ht_export_to_array(prog->environment);
	if (prog->array_env == NULL)
	{
		print_error("Enviroment initialization failed\n");
		return (ERROR);
	}
	prog->saved_term = term;
	prog->cmd_count = 0;
	prog->pipe_count = 0;
	prog->pipes = NULL;
	prog->children = NULL;
	prog->should_execute = TRUE;
	return (SUCCESS);
}
