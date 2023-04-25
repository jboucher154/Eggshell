/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:46:58 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/25 15:43:19 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"

char	*quote_cut(char *to_check_str)
{
	char *new;
	
	new = ft_substr(to_check_str, 1, ft_strlen(to_check_str) - 2);
	if (!new)
	{
		printf("%sEggShell🥚: malloc failed%s\n", RED, KNRM);
		return (NULL);
	}
	return (new);
}

void	insert_new_value(char *str, char *new_str, char *variable, char *value)
{
	int		value_index;
	int		new_index;
	int		old_index;

	value_index = 0;
	new_index = 0;
	old_index = 0;
	while (str[old_index])
	{
		if (str[old_index] == '$')
		{
			while (value && value[value_index])
			{
				new_str[new_index] = value[value_index];
				value_index++;
				new_index++;
			}
			old_index += ft_strlen(variable);
			break ;
		}
		else
			new_str[new_index++] = str[old_index++];
	}
	while (str[old_index])//should pick up after the variable name, finnish copying
	{
		new_str[new_index++] = str[old_index++];
	}
	new_str[new_index] = '\0';
}

//exands the variable
char	*expand_env_var(t_eggcarton *prog_info, char *str, char *variable_start)
{
	char	*variable;
	char	*value;
	char	*new_str;

	if (variable_start == NULL)
		return (str);
	variable =  ft_substr(variable_start, 0, find_end_word(variable_start));
	value  = ht_get(prog_info->environment, variable + 1);
	new_str = (char *) malloc(sizeof(char) * (ft_strlen(str) - ft_strlen(variable) + ft_strlen(value) + 1));
	if (!new_str)
	{
		free(variable);
		printf("%sEggShell🥚: variable expansion failed%s\n", RED, KNRM);
		return (str); //return the origional string incase of failure
	}
	insert_new_value(str, new_str, variable, value);
	free(variable);
	free(str);
	return (expand_env_var(prog_info, new_str, ft_strchr(new_str, '$')));
}

//search for needed expansions
int	check_for_expansions(t_eggcarton *prog_info, char **to_check_arr, int file_flag)
{
	int		index;
	char	*variable_start;
	char	*temp;
	
	index = 0;
	while (to_check_arr[index])
	{
		variable_start = ft_strchr(to_check_arr[index], '$');
		if (variable_start && to_check_arr[index][0] != '\'')
		{
			to_check_arr[index] = expand_env_var(prog_info, to_check_arr[index], variable_start);
			if (to_check_arr[index] == NULL)
				return (ERROR); //
		}
		if (ft_strchr(QUOTES, to_check_arr[index][0]))
		{
			temp = quote_cut(to_check_arr[index]);
			free(to_check_arr[index]);
			to_check_arr[index] = temp; //shoudl we bail here if the malloc failed?
		}
		if (file_flag == 1)
			break;
		index++;
	}
	return (SUCCESS);
}