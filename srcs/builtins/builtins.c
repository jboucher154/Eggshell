/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:54:56 by smorphet          #+#    #+#             */
/*   Updated: 2023/05/18 20:09:35 by jebouche         ###   ########.fr       */
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

int	echo_command(char **args)
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
		ft_putstr_fd(args[index], 1);
		ft_putchar_fd(' ', 1);
		index++;
	}
	if (new_line != 1)
		ft_putchar_fd('\n', 1);
	return (SUCCESS);
}

int	pwd_command(void)
{
	char	*current_wd;

	current_wd = getcwd(NULL, 0);
	ft_putstr_fd(current_wd, 1);
	ft_putchar_fd('\n', 1);
	free(current_wd);
	return (SUCCESS);
}

static char	*find_cd_dest(char	**args, t_eggcarton *prog_info)
{
	char	*dest;

	if (args[1] == NULL)
	{	
		dest = ht_get(prog_info->environment, "HOME");
		if (!dest)
		{
			print_error("HOME not set");
			return (NULL);
		}
		if (dest[0] == '\0')
			return (NULL);
	}
	else
		dest = args[1];
	return (dest);
}

int	cd_command(char	**args, t_eggcarton *prog_info)
{
	char	*current_wd;
	char	*dest;
	char	*error;
	int		exit_status;

	exit_status = EXIT_SUCCESS;
	current_wd = getcwd(NULL, 0);
	ht_update_value(prog_info->environment, "OLDPWD", current_wd);
	dest = find_cd_dest(args, prog_info);
	if (dest == NULL)
		return (EXIT_FAILURE);
	if (chdir(dest) == -1)
	{
		error = strerror(errno);
		print_error(error);
		exit_status = EXIT_FAILURE;
	}
	current_wd = getcwd(NULL, 0);
	ht_update_value(prog_info->environment, "PWD", current_wd);
	return (exit_status);
}
