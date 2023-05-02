/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:46:58 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/02 12:54:49 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	insert_new_value(char *str, char *new_str, char *variable, char *value)
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

int	find_end_of_variable(char *varaible_start)
{
	int	index;

	index = 1;
	while (varaible_start[index] == '_' || ft_isalpha(varaible_start[index]) || \
	ft_isdigit(varaible_start[index]))
		index++;
	return (index);
}

//exands the variable
char	*expand_env_var(t_eggcarton *prog_info, char *str, char *variable_start)
{
	char	*variable;
	char	*value;
	char	*new_str;
	int		new_strlen;

	if (variable_start == NULL)
		return (str);
	variable =  ft_substr(variable_start, 0, find_end_of_variable(variable_start)); // there is whitespace at the end NEEDS TO BE ws or quote
	if (variable == NULL)
		return (str); //ERROR print?
	value = ht_get(prog_info->environment, variable + 1);
	if (value == NULL)
		new_strlen = (ft_strlen(str) - ft_strlen(variable));
	else 
		new_strlen = (ft_strlen(str) - ft_strlen(variable) + ft_strlen(value));
	new_str = (char *) malloc(sizeof(char) * (new_strlen + 1));
	if (!new_str)
	{
		free(variable);
		print_error("variable expansion failed");
		return (str);
	}
	insert_new_value(str, new_str, variable, value);
	free(variable);
	free(str);
	return (new_str);
}

//search for needed expansions
char	*check_for_expansions(t_eggcarton *prog_info, char *str_to_assess)
{
	int		index;
	// t_quote_tracker quote_info;//
	int		in_quote;
	char	quote_type;

	in_quote = FALSE;
	index = 0;
	quote_type = UNSET;
	if (ft_strchr(str_to_assess, '$') == NULL)
		return (str_to_assess);
	while (str_to_assess[index])
	{
		if (ft_strchr(QUOTES, str_to_assess[index]) && in_quote == FALSE)
		{
			in_quote = TRUE;
			quote_type = str_to_assess[index];
		}
		else if (str_to_assess[index] == quote_type && in_quote == TRUE)
		{
			in_quote = FALSE;
			quote_type = UNSET;
		}
		else if (str_to_assess[index] == '$')
		{
			if (((in_quote == TRUE && quote_type != '\'') || in_quote == FALSE) && (str_to_assess[index + 1] != '\0' || (str_to_assess[index + 1] && ft_strchr(WHITESPACE, str_to_assess[index + 1]) == NULL)))
			{
				str_to_assess = expand_env_var(prog_info, str_to_assess, str_to_assess + index);
				if (str_to_assess == NULL)
					return (NULL); //
				index = 0;
				continue ;
			}
		}
		index++;
	}
	return (str_to_assess);
}
