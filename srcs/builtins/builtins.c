/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:54:56 by smorphet          #+#    #+#             */
/*   Updated: 2023/05/03 15:20:25 by jebouche         ###   ########.fr       */
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

void	cd_command(char	**args, t_eggcarton *prog_info)
{
	char	*current_wd;
	char	*to;
	char	*error;

	current_wd = getcwd(NULL, 0);
	ht_update_value(prog_info->environment, "OLDPWD", (void *) current_wd);
	if (args[1] == '\0')
		to = getenv("HOME");
	else
		to = args[1];
	if (chdir(to) == -1)
	{
		error = strerror(errno);
		print_error(error);
		ht_update_value(prog_info->environment, "?", ft_itoa(errno));
	}
	current_wd = getcwd(NULL, 0);
	ht_update_value(prog_info->environment, "PWD", (void *) current_wd);
	return ;
}

int	only_digits(char *to_check)
{
	int	i;

	i = 0;
	while (ft_isdigit(to_check[i]) && to_check[i] != '\0')
		i++;
	if (to_check[i] != '\0')
		return (1);
	return (0);
}

void	clean_and_restore(t_eggcarton *prog_info)
{
	free_children(prog_info->children);
	ht_destroy(&(prog_info->environment));
	ht_destroy(&(prog_info->command_table));
	
	tcsetattr(STDIN_FILENO, TCSANOW, prog_info->saved_term);//
}

void	exit_command(t_eggcarton *prog_info, t_child *cmd)
{
	int exit_code;

	exit_code = 0;
	if (cmd->args[1] != '\0' && cmd->args[2] == '\0')
	{
		if (only_digits(cmd->args[1]))
			print_error("exit: numeric argument required");
		else
			exit_code = ft_atoi(cmd->args[1]);
	}
	else if (cmd->args[2] != '\0')
	{
		print_error("Too many arguments");
	 	printf("exit\n");
		return ;
	}
	else
		exit_code = ft_atoi(ht_get(prog_info->environment, "?"));
	printf("error code is %i\n", exit_code);
	printf("exit\n");
	clean_and_restore(prog_info);
	exit (exit_code);//
}
