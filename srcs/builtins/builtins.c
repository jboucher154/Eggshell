/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:54:56 by smorphet          #+#    #+#             */
/*   Updated: 2023/05/18 13:15:38 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_hash.h"

int	check_if_flag(char *to_check)
{
	size_t	index;

	index = 1;
	if (!to_check || !to_check[0] || to_check[0] != '-')
		return (FALSE);
	while (to_check[index] == 'n')
		index++;
	if (index != ft_strlen(to_check))
		return (FALSE);
	return (TRUE);
}

void	echo_command(char **args)
{
	int		new_line;
	int		index;

	new_line = 0;
	index = 1;
	while (check_if_flag(args[index]) == TRUE)
	{
		new_line = 1;
		index++;
	}
	while (args[index] != NULL)
	{
		printf("%s ", args[index]);
		index++;
	}
	if (new_line != 1)
		printf("\n");
}

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
	ht_update_value(prog_info->environment, "OLDPWD", current_wd);
	if (args[1] == NULL)
	{	
		to = ht_get(prog_info->environment, "HOME");
		if (!to)
		{
			print_error("HOME not set");
			ht_update_value(prog_info->environment, "?", ft_itoa(1));
			return ;
		}
		if (to[0] == '\0')
			return ;
	}
	else
		to = args[1];
	if (chdir(to) == -1)
	{
		error = strerror(errno);
		print_error(error);
		ht_update_value(prog_info->environment, "?", ft_itoa(1));
	}
	current_wd = getcwd(NULL, 0);
	ht_update_value(prog_info->environment, "PWD", current_wd);
	return ;
}
// the update isn't staying for the value updates because 
// it needs to be an exit value for the child processes
