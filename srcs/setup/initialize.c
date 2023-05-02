/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:16:07 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/28 15:50:23 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//should we update the shell program varaible $SHELL as well?
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
	new_level = ft_itoa(int_level);
	if (!new_level)
		return (ERROR);
	return (ht_update_value(environment, "SHLVL", new_level));
}

static void initialize_env_table(t_hash_table *ht_env, char **envp)
{
	int index;
	int out_index;
	char *key;
	char *value;
	
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
	ht_update_value(ht_env, "SHELL", getcwd(NULL, 0));
	if (set_shell_level(ht_env) == ERROR)
		print_error("SHELL LEVEL NOT SET!");
}

int	initialize_eggcarton(t_eggcarton *prog_info, char **envp, struct termios *saved_term)
{
	prog_info->environment = ht_create(150);
	if (prog_info->environment == NULL)
		return (ERROR);
	initialize_env_table(prog_info->environment, envp);
	if (initalize_command_table(prog_info) == ERROR)
		return (EXIT_FAILURE); //clean exit
	prog_info->saved_term = saved_term;
	prog_info->cmd_count = 0;
	prog_info->pipe_count = 0;
	prog_info->og_env = envp;
	prog_info->pipes = NULL;
	prog_info->pids = NULL;
	prog_info->children = NULL;
	return (SUCCESS);
}
