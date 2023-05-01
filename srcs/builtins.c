/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:54:56 by smorphet          #+#    #+#             */
/*   Updated: 2023/05/01 19:24:53 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_hash.h"

// we need to set up child processes
void	echo_command(char **args)
{
	int		new_line;

	new_line = 0;
	args++;
	if (!ft_strncmp("-n", *args, 2))
	{
		new_line = 1;
		while (!ft_strncmp("-n", *args, 2))
			args++;
	}
	while (*args != '\0')
	{
		printf("%s ", *args);
		args++;
	}
	if (new_line != 1)
		printf("\n");
}

void	pwd_command(void)
{
	char	*current_wd;

	current_wd = getcwd(NULL, 0);//check it it needs free
	printf("%s\n", current_wd);
	free(current_wd);
}

void	cd_command(char	**args, t_hash_table *ht_env)
{
	char	*current_wd;
	char	*to;
	char	*error;

	current_wd = getcwd(NULL, 0);
	ht_update_value(ht_env, "OLDPWD", (void *) current_wd);
	
	if (args[1] == '\0')
		to = getenv("HOME");
	else
		to = args[1];
	if (chdir(to) == -1)
	{
		error = strerror(errno);
		print_error(error);
	}
	current_wd = getcwd(NULL, 0);
	ht_update_value(ht_env, "PWD", (void *) current_wd);
	return ;
}

void	unset_command(char **args, t_hash_table *ht_env)
{
	//needs to handle multiple unsets in one call
	int	index;

	index = 1;
	while (args[index] != '\0')
	{	
		ht_remove(ht_env, args[index]);
		index++;
	}
}


void	export_command(char **args, t_hash_table *environment)
{
	int		index;
	int		arg_index;
	char 	*key;
	char	*value;

	index = 1;
	if (!args[index])
	{
		ht_print_export(environment);
		return ;
	}
	while (args[index])
	{
		arg_index = 0;
		while (args[index][arg_index] != '\0' && args[index][arg_index] != '=')
			arg_index++;
		key = ft_substr(args[index], 0, arg_index);
		if (args[index][arg_index] == '\0')
			value = NULL;
		else
		{
			arg_index++;
			value = ft_strdup(args[index]+ arg_index);
		}
		ht_add(environment, key, value);
		free(key);
		index++;
	}
}

void	print_enviroment(t_hash_table	*ht_env)
{
	ht_print_env(ht_env);
}

void	clearing(void)
{
	printf("\033[H\033[J");
}
