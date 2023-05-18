/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:38:37 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/18 13:51:09 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//no access to ?
int	is_valid_var_name(char *key)
{
	size_t	index;

	index = 0;
	if (ft_isdigit(key[0]) || !key[0])
		return (FALSE);
	while (key[index] == '_' || ft_isalpha(key[index]) || \
	ft_isdigit(key[index]))
		index++;
	if (index == ft_strlen(key))
		return (TRUE);
	else
		return (FALSE);
}

void	set_new_env_variable(char *arg, t_hash_table *environment, \
int *error_occured)
{
	int		arg_index;
	char	*key;
	char	*value;

	arg_index = 0;
	while (arg[arg_index] != '\0' && arg[arg_index] != '=')
		arg_index++;
	key = ft_substr(arg, 0, arg_index);
	if (is_valid_var_name(key) == FALSE)
	{
		print_blame_error("export", "not a valid identifier", key);
		free(key);
		(*error_occured) = 1;
		return ;
	}
	if (arg[arg_index] == '\0')
		value = NULL;
	else
	{
		arg_index++;
		value = ft_strtrim(arg + arg_index, WHITESPACE);
	}
	ht_add(environment, key, value);
	free(key);
}

void	export_command(char **args, t_eggcarton *prog_info)
{
	int	index;
	int	added;
	int	error;

	added = 0;
	index = 1;
	error = 0;
	while (args[index])
	{
		if (args[index]) // removed [0]
		{
			set_new_env_variable(args[index], prog_info->environment, &error);
			added++;
		}
		index++;
	}
	if (added == 0)
		ht_print_export(prog_info->environment);
	else
		update_environment_array(prog_info);
	ht_update_value(prog_info->environment, "?", ft_itoa(error));
}
