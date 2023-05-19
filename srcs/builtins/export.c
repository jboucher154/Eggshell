/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:38:37 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 13:40:11 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
*  get_key() gets the key from the argument passed and sets the arg_index
*  pointer to after the key. If valid, the found variable name is returned as
*  the key. Otherwise, it prints an error and returns NULL.
*/
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

/*
*  set_new_env_variable() takes an argument and adds it to the environment
*  hashtable. If the argument is not a valid variable name it prints an error
*  and sets the error_occured pointer to ERROR(1).
*  Otherwise, it adds the variable to the environment hashtable.
*/
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
		(*error_occured) = ERROR;
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

/*
*  export_command takes a char** of arguments and adds them to the 
*  environment hashtable. If the argument is no arguments are passed,
*  it prints the environment variables in the format required by POSIX.
*  It returns SUCCESS if no errors occured, and 1 if an error occured.
*/
int	export_command(char **args, t_eggcarton *prog_info)
{
	int	index;
	int	added;
	int	error;

	added = 0;
	index = 1;
	error = SUCCESS;
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
