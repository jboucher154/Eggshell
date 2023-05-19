/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:38:37 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/18 19:13:30 by jebouche         ###   ########.fr       */
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

static char	*get_key(char *arg, int *arg_index)
{
	char	*key;

	while (arg[*arg_index] != '\0' && arg[*arg_index] != '=')
		(*arg_index)++;
	key = ft_substr(arg, 0, *arg_index);
	if (is_valid_var_name(key) == FALSE)
	{
		if (arg[0] == '=')
			print_blame_error("export", "not a valid identifier", arg);
		else
			print_blame_error("export", "not a valid identifier", key);
		free(key);
		return (NULL);
	}
	return (key);
}

void	set_new_env_variable(char *arg, t_hash_table *environment, \
int	*error_occured)
{
	int		arg_index;
	char	*key;
	char	*value;

	arg_index = 0;
	key = get_key(arg, &arg_index);
	if (key == NULL)
	{
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
	if (ht_get(environment, key) != NULL && value == NULL)
	{
		free(key);
		return ;
	}
	ht_add(environment, key, value);
	free(key);
}

int	export_command(char **args, t_eggcarton *prog_info)
{
	int	index;
	int	added;
	int	error;

	added = 0;
	index = 1;
	error = 0;
	while (args[index])
	{
		set_new_env_variable(args[index], prog_info->environment, &error);
		added++;
		index++;
	}
	if (added == 0)
		ht_print_export(prog_info->environment);
	else
		update_environment_array(prog_info);
	return (error);
}
