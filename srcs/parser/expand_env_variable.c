/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_variable.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:10:17 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 14:21:23 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * copy_from_value takes in pointers to the new string, the current index in the
 * new string and the value. It copies the value into the new string.
 */
static void	copy_from_value(char *new_str, int *new_index, char *value)
{
	int	value_index;

	value_index = 0;
	while (value && value[value_index])
	{
		new_str[*new_index] = value[value_index];
		value_index++;
		(*new_index)++;
	}
}

/*
 * insert_new_value takes in pointers to the input string, the new string, the
 * variable and the value. It inserts the value into the new string and returns
 * the new string.
 */
static void	insert_new_value(char *str, char *new_str, char *variable, \
char *value)
{
	int		new_index;
	int		old_index;

	new_index = 0;
	old_index = 0;
	while (str[old_index])
	{
		if (str[old_index] == '$')
		{
			copy_from_value(new_str, &new_index, value);
			old_index += ft_strlen(variable);
			break ;
		}
		else
			new_str[new_index++] = str[old_index++];
	}
	while (str[old_index])
	{
		new_str[new_index++] = str[old_index++];
	}
	new_str[new_index] = '\0';
}

/*
 * find_end_of_var takes in a pointer to the start of a variable. It returns
 * the index of the end of the variable.
 */
int	find_end_of_var(char *varaible_start)
{
	int	index;

	index = 1;
	if (varaible_start[index] == '?' || ft_isdigit(varaible_start[index]))
		return (index + 1);
	while (varaible_start[index] == '_' || ft_isalpha(varaible_start[index]) || \
	ft_isdigit(varaible_start[index]))
		index++;
	return (index);
}

/*
 * new_str_len takes in pointers to the input string, the variable and the value.
 * It returns the length of the new string after the variable has been expanded.
 */
static size_t	new_str_len(char *str, char *variable, char *value)
{
	if (value == NULL)
		return (ft_strlen(str) - ft_strlen(variable));
	else
		return (ft_strlen(str) - ft_strlen(variable) + ft_strlen(value));
}

/*
 * expand_env_var takes in pointers to the input string, the start of the variable,
 * the t_eggcarton struct and a pointer to the current index in the input string. 
 * It expands the variable and returns the expanded string. If a variable is not
 * found in the environment, it returns the original string.
 */
char	*expand_env_var(t_eggcarton *prog_info, char *str, char *var_start, \
int *index)
{
	char	*var;
	char	*value;
	char	*new_str;

	if (var_start == NULL)
		return (str);
	var = ft_substr(var_start, 0, find_end_of_var(var_start));
	if (var == NULL)
	{
		print_error("malloc failed during expansion");
		return (str);
	}
	value = ht_get(prog_info->environment, var + 1);
	new_str = ft_calloc((new_str_len(str, var, value) + 1), sizeof(char));
	if (!new_str)
	{
		free(var);
		print_error("variable expansion failed");
		return (str);
	}
	insert_new_value(str, new_str, var, value);
	if (value)
		(*index) += ft_strlen(value);
	ft_free_two(var, str);
	return (new_str);
}
