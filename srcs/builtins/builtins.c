/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:54:56 by smorphet          #+#    #+#             */
/*   Updated: 2023/05/03 11:15:58 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_hash.h"

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

//check if return from getcwd needs freed
void	pwd_command(void)
{
	char	*current_wd;

	current_wd = getcwd(NULL, 0);
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

 void	exit_command(t_eggcarton *prog_info, t_child *cmd)
 {
	int exit_code;

	exit_code = 0;
	if (cmd->args[1] == '\0')
		exit_code = ft_atoi(ht_get(prog_info->environment, "?"));
	// else if (cmd->args[1] != '\0' && cmd->args[2] != '\0')
	// {
	// 	print_error("Too many arguments");
	// 	return ;
	// }
	printf("exit\n");
	exit (exit_code);//
 }
